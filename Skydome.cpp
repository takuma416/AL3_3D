#include "skydome.h"

void skydome::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	camera_ = camera;
}

void skydome::Update() {}

void skydome::Draw() { model_->Draw(worldTransform_, *camera_); }


