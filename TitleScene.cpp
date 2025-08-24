#include "MyMath.h"
#include "TitleScene.h"
#include "numbers"

using namespace KamataEngine;

TitleScene::~TitleScene() {
	// モデル
	delete model_;
	delete modelPlayer_;

	delete fade_;
}

void TitleScene::Initialize() {

	// 3Dモデルの生成
	model_ = Model::CreateFromOBJ("titleFont");
	modelPlayer_ = Model::CreateFromOBJ("player");

	// カメラの初期化
	camera_.Initialize();

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = {2, 2, 2};
	worldTransform_.translation_ = {0, 8, 0};

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {10, 10, 10};
	worldTransformPlayer_.translation_ = {0, -8, 0};
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;

	phase_ = Phase::kFadeIn;

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void TitleScene::Update() {

	switch (phase_) {
	case Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
	}

	// アフィン変換行列の作成
	worldTransform_.matWorld_ = MakeaffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// 回転
	rotate += 0.1f;
	worldTransformPlayer_.rotation_.y = sin(rotate) + std::numbers::pi_v<float>;

	// アフィン変換行列の作成
	worldTransformPlayer_.matWorld_ = MakeaffineMatrix(worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_, worldTransformPlayer_.translation_);
	// 行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();
}

void TitleScene::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	// 描画
	model_->Draw(worldTransform_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);
	// 3Dモデル描画語処理
	Model::PostDraw();

	fade_->Draw();
}
