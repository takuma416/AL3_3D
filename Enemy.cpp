#include "MyMath.h"
#include "cassert"
#include "numbers"
#include "player.h"
#define NOMINMAX
#include "Enemy.h"
#include "MapChipField.h"
#include "algorithm"

using namespace KamataEngine;

using namespace MathUtility;

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	camera_ = camera;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;
	// 速度の設定
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() {

	worldTransform_.translation_ += velocity_;

	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;
	// 回転アニメーション
	worldTransform_.rotation_.x = std::sin(walkTimer_ * 5.0f);
	/*float param = std::sin(walkTimer_ * 5.0f);
	float degree = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = ;*/

	worldTransform_.matWorld_ = MakeaffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }

void Enemy::OnCollision(const Player* player) { (void)player; }

KamataEngine::Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

AABB Enemy::GetAABB() {

	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}