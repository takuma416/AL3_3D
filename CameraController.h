//#pragma once
//#include "KamataEngine.h"
//#include "Player.h"
//
//class CameraController {
//public:
//	void Initialize();
//	void Update();
//	void SetTarget(Player* target) { target_ = target; }
//	void Reset();
//	KamataEngine::Vector3 targetoffset_ = {0, 0, -15.0f};
//	KamataEngine::WorldTransform worldTransform_;
//	const KamataEngine::Camera& GetViewProjection() const { return camera_; }
//	struct Rect {
//		float left = 0.0f;
//		float right = 1.0f;
//		float bottom = 0.0f;
//		float top = 1.0f;
//	};
//	Rect movableArea_ = {0, 100, 0, 100};
//	void SetMovableArea(const Rect& area) { movableArea_ = area; }
//	KamataEngine::Vector3 targetPosition_ = {0, 0, 0};
//	static inline const float kInterpolationRate = 0.1f;
//	static inline const float kVelocityBias = 30;
//	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};
//	
//
//private:
//	KamataEngine::Camera camera_;
//
//	//インクルード
//
//	
//	Player* target_ = nullptr;
//
//	class Player;
//	
//};
#pragma once
#include "player.h"
#include <KamataEngine.h>
class CameraController {
public:
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};
	Rect movableArea_ = {0, 100, 0, 100};

	void SetMovableArea(Rect area) { movableArea_ = area; }

	KamataEngine::Vector3 targetOffset_ = {0, 0, -15.0f};
	void SetTarget(Player* target) { target_ = target; }
	void Reset();
	const KamataEngine::Camera& GetViewProjection() const { return camera_; }
	void Initialize();

	static inline const float kVelocityBias = 5;
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};

	KamataEngine::Vector3 targetPosition_;
	static inline const float kInterpolationRate = 0.2f;

	void Update();

private:
	KamataEngine::Camera camera_;

	Player* target_ = nullptr;
};
class Player;
