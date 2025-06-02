#pragma once
#include "KamataEngine.h"
#include "vector"

class Player {
public:
	void Initialize(KamataEngine::Model* model,  KamataEngine::Camera* camera,const KamataEngine::Vector3& position);
	void Update();
	void Draw();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	/*uint32_t textureHandle_ = 0u;*/
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Vector3 velocity_{};
	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 2.0f;
};