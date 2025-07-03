#include "GameScene.h"
#include "MapChipField.h"
#include "MyMath.h"
#include "Player.h"
#include "Skydome.h"
#include "CameraController.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::CreateFromOBJ("player");
	modelBlock_ = Model::CreateFromOBJ("block");
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	camera_.Initialize();

	skydome_ = new Skydome();
	skydome_->Intialize(modelSkydome_, textureHandle_, &camera_);
	player_ = new Player();

	debugCamera_ = new DebugCamera(1280, 720);
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	Vector3 playerPosition = mapChipField_->GetMapChipPositionBiIndex(1, 18);
	//playerPosition = {0, 0, 0};
	player_->Initialize(model_, &camera_, playerPosition);

	//カメラコントローラーの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);
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
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		camera_.TransferMatrix();
	}

	cameraController_->Update();
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
	skydome_->Draw();
	player_->Draw();
	Model::PostDraw();
}

void GameScene::GenerateBlocks() {
	uint32_t kNumBlockVertical = mapChipField_->GetNumBlockVirtical();
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	worldTransformBlocks_.resize(kNumBlockVertical);
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransform->translation_ = mapChipField_->GetMapChipPositionBiIndex(j, i);
			}
		}
	}
}

