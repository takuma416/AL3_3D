#include "player.h"
#include "MyMath.h"
#include "cassert"
#include "numbers"
#define NOMINMAX
#include "MapChipField.h"
#include "algorithm"

using namespace KamataEngine;

using namespace MathUtility;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	camera_ = camera;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	// 移動入力
	InputMove();

	// ２移動量を加味して衝突判定する
	CollisionMapInfo collisionMapInfo;

	collisionMapInfo.move = velocity_;

	CheckMapCollision(collisionMapInfo);

	CheckMapMove(collisionMapInfo);

	CheckMapCeiling(collisionMapInfo);

	CheckMapLanding(collisionMapInfo);

	/*bool landing = false;

	if (velocity_.y < 0) {
	    if (worldTransform_.translation_.y <= 1.0f) {
	        landing = true;
	    }
	}

	if (onGround_) {
	    if (velocity_.y > 0.0f) {
	        onGround_ = false;
	    }
	} else {
	    if (landing) {
	        worldTransform_.translation_.y = 1.0f;

	        velocity_.x *= (1.0f - kAttenuation);

	        velocity_.y = 0.0f;

	        onGround_ = true;
	    }
	}*/

	//
	AnimateTurn();

	worldTransform_.matWorld_ = MakeaffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }

void Player::InputMove() {

	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
			velocity_ += acceleration;

			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_.y += kJumpAcceleration;
			onGround_ = false;
		}
	} else {
		velocity_ += Vector3(0, -kGravityAcceleration, 0);

		velocity_.y = max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::CheckMapCollision(CollisionMapInfo& info) {
	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info) {

	if (info.move.y <= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;

	MapChipField::IndexSet IndexSet;
	IndexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
	/*mapChipType = mapChipField_->GetMapChipPositionByIndex(IndexSet.xIndex, IndexSet.yIndex);*/
	mapChipType = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	IndexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		IndexSet = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.move + KamataEngine::Vector3(0, +kHeight / 2.0f, 0));

		MapChipField::IndexSet IndexSetNow;
		IndexSetNow = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + KamataEngine::Vector3(0, +kHeight / 2.0f, 0));
		if (IndexSetNow.yIndex != IndexSet.yIndex) {

			MapChipField::Rect rect = mapChipField_->GetRectByIndex(IndexSet.xIndex, IndexSet.yIndex);
			info.move.y = max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));

			info.ceiling = true;
		}
	}
}

void Player::CheckMapMove(const CollisionMapInfo& info) { worldTransform_.translation_ += info.move; }

void Player::CheckMapCeiling(const CollisionMapInfo& info) {

	if (info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit Ceiling\n");
		velocity_.y = 0;
	}
}

void Player::CheckMapLanding(const CollisionMapInfo& info) {

	// 自キャラが接地状態
	if (onGround_) {

		if (velocity_.y > 0.0f) {
			onGround_ = false;

		} else {
			bool hit = false;
			if (!hit) {

				std::array<Vector3, kNumCorner> positionsNew;

				for (uint32_t i = 0; i < positionsNew.size(); ++i) {
					positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
				}

				MapChipType mapChipType;

				MapChipField::IndexSet IndexSet;
				IndexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kLeftBottom] + KamataEngine::Vector3(0, -kGroundSearchHeight, 0));
				/*mapChipType = mapChipField_->GetMapChipPositionByIndex(IndexSet.xIndex, IndexSet.yIndex);*/
				mapChipType = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex);

				if (mapChipType == MapChipType::kBlock) {
					hit = true;
				}

				IndexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kRightBottom] + KamataEngine::Vector3(0, -kGroundSearchHeight, 0));
				mapChipType = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex);
				if (mapChipType == MapChipType::kBlock) {
					hit = true;
				}

				onGround_ = false;
			}
		}

	} else {

		if (info.landing) {

			onGround_ = true;

			velocity_.x *= (1.0f - kAttenuationLanding);

			velocity_.y = 0.0f;
		}
	}
}

void Player::CheckMapWall(const CollisionMapInfo& info) {
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

void Player::CheckMapCollisionDown(CollisionMapInfo& info) {
	if (info.move.y >= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;

	MapChipField::IndexSet IndexSet;
	IndexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
	/*mapChipType = mapChipField_->GetMapChipPositionByIndex(IndexSet.xIndex, IndexSet.yIndex);*/
	mapChipType = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	IndexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		IndexSet = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.move + KamataEngine::Vector3(0, -kHeight / 2.0f, 0));

		MapChipField::IndexSet IndexSetNow;
		IndexSetNow = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + KamataEngine::Vector3(0, -kHeight / 2.0f, 0));
		if (IndexSetNow.yIndex != IndexSet.yIndex) {

			MapChipField::Rect rect = mapChipField_->GetRectByIndex(IndexSet.xIndex, IndexSet.yIndex);
			info.move.y = min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));

			info.landing = true;
		}
	}
}

void Player::CheckMapCollisionRight(CollisionMapInfo& info) {

	if (info.move.x <= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;

	MapChipField::IndexSet IndexSet;
	IndexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kRightTop]);
	/*mapChipType = mapChipField_->GetMapChipPositionByIndex(IndexSet.xIndex, IndexSet.yIndex);*/
	mapChipType = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex - 1, IndexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	IndexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex - 1, IndexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		IndexSet = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.move + KamataEngine::Vector3(+kWidth / 2.0f, 0, 0));

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(IndexSet.xIndex, IndexSet.yIndex);
		info.move.x = max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));

		info.hitWall = true;
	}
}

void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {

	if (info.move.x >= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;

	MapChipField::IndexSet IndexSet;
	IndexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
	/*mapChipType = mapChipField_->GetMapChipPositionByIndex(IndexSet.xIndex, IndexSet.yIndex);*/
	mapChipType = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex + 1, IndexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	IndexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex, IndexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(IndexSet.xIndex + 1, IndexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		IndexSet = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.move + KamataEngine::Vector3(-kWidth / 2.0f, 0, 0));

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(IndexSet.xIndex, IndexSet.yIndex);
		info.move.x = min(0.0f, rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));

		info.hitWall = true;
	}
}

void Player::AnimateTurn() {

	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状況に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}

KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0},
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}
