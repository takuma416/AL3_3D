#include "DeathParticles.h"
#include "MyMath.h"
#include "algorithm"

using namespace KamataEngine;

using namespace MathUtility;

void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

	for (WorldTransform& worldTransform : worldTransform_) {
		worldTransform.Initialize();

		worldTransform.translation_ = position;
	}
	camera_ = camera;
}

void DeathParticles::Update() {

	for (WorldTransform& worldTransform : worldTransform_) {
		worldTransform.matWorld_ = MakeaffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);

		worldTransform.TransferMatrix();
	}
	for (uint32_t i = 0; i < 8; i++) {

		KamataEngine::Vector3 velocity = {0.1f, 0, 0};

		float angle = kAngleUnit * i;

		Matrix4x4 matriRotation = MakeRotateZMatrix(angle);

		velocity = Transform(velocity, matriRotation);

		worldTransform_[i].translation_ += velocity;
	}
	counter_ += 1.0f / 60.0f;

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);

	objectColor_.SetColor(color_);

	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished_ = true;
	}
	if (isFinished_) {
		return;
	}
}

void DeathParticles::Draw() {
	for (WorldTransform& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, *camera_, &objectColor_);
	}
	if (isFinished_) {
		return;
	}
}
