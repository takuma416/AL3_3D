#pragma once
#include "KamataEngine.h"

class Fade {
public:
	// フェードの状態
	enum class Status {
		None,    // フェードなし
		FadeIn,  // フェードイン
		FadeOut, // フェードアウト
	};

	void Initialize();

	void Update();

	void Draw();

	void Start(Status status, float duration);

	void Stop();

	bool IsFinished() const;

private:
	uint32_t textureHandle_ = 0;

	KamataEngine::Sprite* sprite_ = nullptr;
	// 現在のフェードの状態
	Status status_ = Status::None;

	// フェードの持続時間
	float duration_ = 0.0f;
	// 経過時間カウンター
	float counter_ = 0.0f;
};
