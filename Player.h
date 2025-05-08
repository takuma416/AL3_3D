#pragma once
#include "KamataEngine.h"

class Player {
public:
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);
	void Update();
	void Draw();
};
