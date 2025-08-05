#pragma once
#include "KamataEngine.h"

class Enemy {

public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

private:
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;
	
	static inline const float kWalkSpeed = 0.05f;
	
	KamataEngine::Vector3 velocity_ = {};
	
	static inline const float kWalkMotionAngleStart = 0.1f;
	
	static inline const float kWalkMotionAngleEnd = 0.5f;
	
	static inline const float kWalkMotionTime = 0.2f;
	
	float walkTimer_ = 0.0f;
};
