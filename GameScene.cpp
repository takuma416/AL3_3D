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

	// 2次元配列を作成
	worldTransformBlocks_.resize(kNumBlockHorizontal);
	for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
		worldTransformBlocks_[x].resize(kNumBlockVertical);
		for (uint32_t y = 0; y < kNumBlockVertical; ++y) {
			worldTransformBlocks_[x][y] = new WorldTransform();
			worldTransformBlocks_[x][y]->Initialize();
			worldTransformBlocks_[x][y]->translation_.x = kBlockWidth * x;
			worldTransformBlocks_[x][y]->translation_.y = kBlockHeight * y;
		}
	}
}

void GameScene::Update() {
	player_->Update();

	for (uint32_t x = 0; x < worldTransformBlocks_.size(); ++x) {
		for (uint32_t y = 0; y < worldTransformBlocks_[x].size(); ++y) {
			WorldTransform* worldTransformBlock = worldTransformBlocks_[x][y];
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			worldTransformBlock->TransferMatrix();
		}
	}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());

	/*player_->Draw();*/

	for (uint32_t x = 0; x < worldTransformBlocks_.size(); ++x) {
		for (uint32_t y = 0; y < worldTransformBlocks_[x].size(); ++y) {
			modelBlock_->Draw(*worldTransformBlocks_[x][y], camera_);
		}
	}

	Model::PostDraw();
}
