#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class MapChipField;

class Enemy;

class Player {
public:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	/*void Initialize(KamataEngine::Model* model,uint32_t textureHandle,KamataEngine::Camera* camera);*/
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	AABB GetAABB();

	KamataEngine::Vector3 GetWorldPosition();

	void OnCollision(const Enemy* enemy);

private:
	// スプライト
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	KamataEngine::Vector3 velocity_ = {};

	KamataEngine::WorldTransform worldTransform_;

	static inline const float kAcceleration = 0.1f;

	static inline const float kAttenuation = 0.1f;

	static inline const float kLimitRunSpeed = 1.0f;

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;

	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;

	static inline const float kGravityAcceleration = 0.1f;

	static inline const float kLimitFallSpeed = 1.0f;

	static inline const float kJumpAcceleration = 1.0f;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		KamataEngine::Vector3 move;
	};

	void InputMove();

	void CheckMapCollision(CollisionMapInfo& info);
	// 衝突判定　上
	void CheckMapCollisionUp(CollisionMapInfo& info);
	// 衝突判定　下
	void CheckMapCollisionDown(CollisionMapInfo& info);
	// 衝突判定　右
	void CheckMapCollisionRight(CollisionMapInfo& info);
	// 衝突判定　左
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	void CheckMapMove(const CollisionMapInfo& info);

	void CheckMapCeiling(const CollisionMapInfo& info);

	void CheckMapLanding(const CollisionMapInfo& info);

	void CheckMapWall(const CollisionMapInfo& info);

	void AnimateTurn();

	// 角
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner
	};

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

	static inline const float kBlank = 0.1f;

	static inline const float kAttenuationLanding = 0.5f;

	static inline const float kGroundSearchHeight = 0.5f;

	static inline const float kAttenuationWall = 0.5f;
};