#include "GameScene.h"
#include "MyMath.h"
#include "player.h"

using namespace KamataEngine;

// デストラクタ(解放)
GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete modelEnemy_;
	delete modelSkydome_;
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockline : worldTransformBlocks_) {
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockline) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete deathParticles_;
	delete mapChipField_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete fade_;
}

// 初期化処理
void GameScene::Initialize() {

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);

	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(20, 18);
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");
	// スプライトインスタンスの生成
	model_ = Model::CreateFromOBJ("block", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	modelParticles_ = Model::CreateFromOBJ("deathParticle", true);

	debugCamera_ = new DebugCamera(1280, 720);

	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	/*deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelParticles_, &camera_, playerPosition);*/

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	// ゲームプレイフェーズから開始
	phase_ = Phase::kFadeIn;

	GenerateBlocks();

	// カメラの初期化
	camera_.Initialize();

	// 自キャラにの生成
	player_ = new Player();

	// 　敵キャラの生成
	/*enemy_ = new Enemy();*/
	for (int32_t i = 0; i < 2; i++) {
		Enemy* newEnemy = new Enemy();
		enemyPosition = mapChipField_->GetMapChipPositionByIndex(20 + i, 18);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	/*mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");*/
	GenerateBlocks();
	player_->SetMapChipField(mapChipField_ );

	skydome_ = new skydome();

	cameraController_ = new CameraController();

	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	// 敵キャラの初期化
	/*enemy_->Initialize(modelEnemy_,&camera_,enemyPosition);*/

	// 背景
	skydome_->Initialize(modelSkydome_, textureHandle_, &camera_);

	cameraController_->Initialize();

	cameraController_->SetTarget(player_);

	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

// 更新処理
void GameScene::Update() {

	switch (phase_) {
	case GameScene::Phase::kPlay:
		// すべての当たり判定を行う
		CheckAllCollisions();
		if (player_->IsDead() == true) {
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;
			//
			deathParticles_->Initialize(modelParticles_, &camera_, deathParticlesPosition);
		}
		break;
	case GameScene::Phase::kDeath:

		deathParticles_->Update();

		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeout;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}

		break;
	case GameScene::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kPlay;
		}
		break;
	case GameScene::Phase::kFadeout:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

	fade_->Update();
	// 自キャラの更新
	player_->Update();
	/*enemy_->Update();*/

	ChangePhase();

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->matWorld_ = MakeaffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			worldTransformBlock->TransferMatrix();
		}
	}
	debugCamera_->Update();
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.TransferMatrix();
	} else {
		///*camera_.UpdateMatrix();*/
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		//
		camera_.TransferMatrix();
	}
	cameraController_->Update();

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	/*if (deathParticles_) {
		deathParticles_->Update();
	}*/
	/*if (deathParticles_ && deathParticles_->IsFinished()) {
		finished_ = true;
	}*/
}

// 描画処理
void GameScene::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// ここに描画処理

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockline : worldTransformBlocks_) {
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockline) {
			if (!worldTransformBlock) {
				continue;
			}
			model_->Draw(*worldTransformBlock, camera_);
		}
	}
	// プレイヤーの表示kPlayの時表示
	if (phase_ == Phase::kPlay) {
		// 自キャラの描画
		player_->Draw();
	}
	// 背景の描画
	skydome_->Draw();
	// 敵の描画
	/*enemy_->Draw();*/

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	fade_->Draw();

	// スプライト描画後処理
	Model::PostDraw();
}

void GameScene::GenerateBlocks() {
	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVerirtical();
	const uint32_t kNumBlockHorizon = mapChipField_->GetNumBlockHorizontal();

	// const float kBlockWidth = 2.0f;
	// const float kBlockheight = 2.0f;

	worldTransformBlocks_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {

		worldTransformBlocks_[i].resize(kNumBlockHorizon);
	}
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizon; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransform->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
	// 判定対象1と2の座標
	AABB aabb1, aabb2;
	// 自キャラの座標
	aabb1 = player_->GetAABB();
	// 自キャラと敵弾全ての当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(enemy);

			// ジャンプ開始(仮処理)
			enemy->OnCollision(player_);
		}
	}
}

void GameScene::ChangePhase() {

	switch (phase_) {
	case GameScene::Phase::kPlay:
		// ゲームプレイフェーズの処理
		if (player_->IsDead()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;
			//
			deathParticles_->Initialize(modelParticles_, &camera_, deathParticlesPosition);
		}
		break;
	case GameScene::Phase::kDeath:
		// デス演出フェーズの処理
		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeout;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}


		break;
	}
}