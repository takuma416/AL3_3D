#pragma once
#include "CameraController.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "player.h"
#include "skydome.h"
#include "vector"

class GameScene {
public:
	~GameScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 表示ブロックの生成
	void GenerateBlocks();

	// すべての当たり判定を行う
	void CheckAllCollisions();

	// スプライト
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Model* modelSkydome_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::Model* modelEnemy_ = nullptr;

	KamataEngine::Camera camera_;

	MapChipField* mapChipField_;

	Player* player_ = nullptr;

	/*Enemy* enemy_ = nullptr;*/

	skydome* skydome_ = nullptr;

	CameraController* cameraController_ = nullptr;

	bool isDebugCameraActive_ = false;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	
	std::list<Enemy*> enemies_;

private:
	uint32_t textureHandle_ = 0;
};