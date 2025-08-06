#pragma once

#include "KamataEngine.h"

KamataEngine::Matrix4x4 MakeaffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation);

float EaseInOut(float x1, float x2, float t);

struct AABB {
	KamataEngine::Vector3 max;
	KamataEngine::Vector3 min;
};
// 衝突判定時
bool IsCollision(const AABB& aabb1, const AABB& aabb2);