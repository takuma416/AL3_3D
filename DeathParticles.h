#pragma once
#include "KamataEngine.h"
#include "array"

class DeathParticles {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	bool IsFinished() const { return isFinished_; }

private:
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	static inline const uint32_t kNumParticles = 8;

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransform_;

	static inline const float kDuration = 1.0f;

	static inline const float kSpeed = 0.1f;

	static inline const float kAngleUnit = 2 * 3.14f / 8;

	bool isFinished_ = false;

	KamataEngine::ObjectColor objectColor_;

	KamataEngine::Vector4 color_;

	float counter_ = 0.0f;
};
