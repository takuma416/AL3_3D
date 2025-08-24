#pragma once
#include "Fade.h"
#include "KamataEngine.h"

class TitleScene {
public:
	// シーンのフェーズ
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();
	// 終了フラグ
	bool finished_ = false;
	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:
	// 3Dモデル

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	// カメラ
	KamataEngine::Camera camera_;
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;
	// 回転
	float rotate = 0.0f;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;
};
