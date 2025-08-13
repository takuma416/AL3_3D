#include <Windows.h>
#pragma once
#include "KamataEngine.h"
#include "GameScene.h"
#include "TitleScene.h"

using namespace KamataEngine;

TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;

void ChangeScene();
void UpdateScene();
void DrawScene();

enum class Scene {

	kUnkown = 0,

	kTitle,
	kGame,
};

Scene scene = Scene::kUnkown;
// ゲームシーン
//class Gamescene {};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"GC2D_04_オガワ_タクマ_AL3");

	scene = Scene::kTitle;
	titleScene = new TitleScene();
	titleScene->Initialize();

	// DirectXcommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// ゲームシーンの更新
		ChangeScene();

		UpdateScene();
		// 描画開始
		dxCommon->PreDraw();

		// ゲームシーンの描画
		DrawScene();

		// 描画終了
		dxCommon->PostDraw();
	}
	// エンジンの終了処理
	KamataEngine::Finalize();
	// ゲームシーンの解放
	delete gameScene;
	delete titleScene;
	// nullptrの代入
	gameScene = nullptr;
	return 0;
}

void ChangeScene() {
	switch (scene) {
	case Scene::kUnkown:
		break;
	case Scene::kTitle:

		if (titleScene->IsFinished()) {
			// シーン切り替え
			scene = Scene::kGame;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}

		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {
			// シーン切り替え
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kUnkown:
		break;
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kUnkown:
		break;
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	}
}
