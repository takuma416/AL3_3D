#include "Skydome.h"

void Skydome::Intialize(KamataEngine::Model* model, uint32_t modelSkydome, KamataEngine::Camera* camera) {
	assert(model);
	model_ = model;
	modelSkydome_ = modelSkydome;
	camera_ = camera;
	worldTransform_.Initialize();
	
}

void Skydome::Update() {}

void Skydome::Draw() {
	if (camera_ && model_) {
		model_->Draw(worldTransform_, *camera_, modelSkydome_);
	}
}


