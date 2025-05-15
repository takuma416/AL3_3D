#include "GameScene.h"
#include "MyMath.h"
#include "Player.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();
	modelBlock_ = Model::CreateFromOBJ("cube");
	camera_.Initialize();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &camera_);

	const uint32_t kNumBlockVertical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	worldTransformBlocks_.resize(kNumBlockHorizontal);
	for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
		worldTransformBlocks_[x].resize(kNumBlockVertical, nullptr);
		for (uint32_t y = 0; y < kNumBlockVertical; ++y) {
			if ((x + y) % 2 == 0) {
				worldTransformBlocks_[x][y] = new WorldTransform();
				worldTransformBlocks_[x][y]->Initialize();
				worldTransformBlocks_[x][y]->translation_.x = kBlockWidth * x;
				worldTransformBlocks_[x][y]->translation_.y = kBlockHeight * y;
			}
		}
	}

	debugCamera_ = new DebugCamera(1280, 720);
}

void GameScene::Update() {
	player_->Update();

	for (uint32_t x = 0; x < worldTransformBlocks_.size(); ++x) {
		for (uint32_t y = 0; y < worldTransformBlocks_[x].size(); ++y) {
			WorldTransform* worldTransformBlock = worldTransformBlocks_[x][y];
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();
		}
	}

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) { 
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());
	for (uint32_t x = 0; x < worldTransformBlocks_.size(); ++x) {
		for (uint32_t y = 0; y < worldTransformBlocks_[x].size(); ++y) {
			if (worldTransformBlocks_[x][y]) {
				modelBlock_->Draw(*worldTransformBlocks_[x][y], camera_);
			}
		}
	}

	Model::PostDraw();
}
