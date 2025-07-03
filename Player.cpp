#define NOMINMAX
#include "Player.h"
#include "MyMath.h"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;
using namespace MathUtility;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// プレイヤーは初期位置で地面にいる状態からスタート
	onGround_ = true;

	// 移動速度の初期化
	velocity_ = Vector3(0.0f, 0.0f, 0.0f);
}

void Player::Update() {
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration{};
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
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		// ジャンプ
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_.y += kJumpAcceleration;
			onGround_ = false;
		}
	} else {
		// 空中では重力がかかる
		velocity_.y -= kGravityAcceleration;
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 着地判定
	bool landing = false;
	if (velocity_.y < 0.0f && worldTransform_.translation_.y <= 3.0f) {
		landing = true;
	}

	if (!onGround_ && landing) {
		worldTransform_.translation_.y = 1.0f;
		velocity_.y = 0.0f;
		velocity_.x *= (1.0f - kAttenuation);
		onGround_ = true;
	}

	// 移動処理
	worldTransform_.translation_ += velocity_;

	// 旋回制御
	if (turnTimer_ > 0.0f) {
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = destinationRotationY;
		turnTimer_ -= 1.0f;
	}

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	if (camera_ && model_) {
		model_->Draw(worldTransform_, *camera_);
	}
}
