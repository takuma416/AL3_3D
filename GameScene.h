#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include<vector>

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();

private:
	uint32_t textureHandle_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;

	std::vector<WorldTransform*> worldTransformBlocks_;
};
