#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "player.h"
#include "skydome.h"
#include "vector"

class GameScene {
public:
	enum class Phase {
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
		kFadeIn,
		kFadeout,
	};
	// ゲームの現在フェーズ
	Phase phase_;

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

	void ChangePhase();

	// スプライト
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Model* modelSkydome_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::Model* modelEnemy_ = nullptr;

	KamataEngine::Model* modelParticles_ = nullptr;

	KamataEngine::Camera camera_;

	MapChipField* mapChipField_;

	Player* player_ = nullptr;

	/*Enemy* enemy_ = nullptr;*/

	skydome* skydome_ = nullptr;

	CameraController* cameraController_ = nullptr;

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }
	// 終了フラグ
	bool finished_ = false;

	bool isDebugCameraActive_ = false;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	/*敵の複数化*/
	std::list<Enemy*> enemies_;

	DeathParticles* deathParticles_ = nullptr;

	Fade* fade_ = nullptr;

private:
	uint32_t textureHandle_ = 0;
};