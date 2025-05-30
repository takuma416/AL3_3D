#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
#include "Skydome.h"
#include "MapChipField.h"

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();
	bool isDebugCameraActive_ = false;
	KamataEngine::Model* modelSkydome_ = nullptr;
	MapChipField* mapChipField_=nullptr;
	void GenerateBlocks();

private:
	uint32_t textureHandle_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelBlock_ = nullptr;
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	Skydome* skydome_ = nullptr;
};
