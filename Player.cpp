#include "Player.h"
#include "MyMath.h"
#include <numbers>
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,const Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.rotation_, 
		worldTransform_.scale_,worldTransform_.translation_);
}

void Player::Update() {
	worldTransform_.TransferMatrix();
	worldTransform_.translation_ += velocity_;
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
	
		Vector3 acceleration{};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
		velocity_ += acceleration;
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	}
}
void Player::Draw() {
	if (camera_ && model_) {
		model_->Draw(worldTransform_, *camera_);
	}
}
