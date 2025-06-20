#include "Application.h"
#include "Game.h"
#include "Input.h"
#include "SceneController.h"
#include <DxLib.h>
#include "EffectManager.h"
#include <EffekseerForDXLib.h>
#include "SoundManager.h"

#if _DEBUG
namespace
{
	// コンソール用
	FILE* in = 0;
	FILE* out = 0;
}
#endif

Application::Application() :
	m_isRunning(true)
{
}

Application& Application::GetInstance()
{
	static Application instance;
	return instance;
}

bool Application::Init()
{
	/*コンソールDebug用*/
#if _DEBUG
	AllocConsole();							// コンソール
	freopen_s(&out, "CON", "w", stdout);	// stdout
	freopen_s(&in, "CON", "r", stdin);		// stdin
#endif

	// 初期化前に必要な情報の定義
	ChangeWindowMode(Game::kInitWindowMode);
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitDepth);

	// DxLibの初期化で、エラったら即異常値を返す
	if (DxLib_Init() == -1) return false;

	// 裏画面に描画します
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	return true;
}

void Application::Run() const
{
	SceneController& scenes = SceneController::GetInstance();
	Input& input = Input::GetInstance();
	EffectManager& effect = EffectManager::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();
	scenes.Init();
	input.Init();
	effect.Init();
	sound.Init();

	// ゲームループ
	while (ProcessMessage() == 0)
	{
		// 開始時のコンピュータの時間を取得
		long long beforFrameTime = GetNowHiPerformanceCount();

		// 画面をきれいに
		ClearDrawScreen();

		input.Update();

		// ここにゲームの処理を書く
		scenes.Update();
		scenes.Draw();

		effect.Update();
		effect.Draw();

		sound.Update();

		// 描画した裏画面を反映
		ScreenFlip();

		// 60fpsになるように調整してる
		while (GetNowHiPerformanceCount() - beforFrameTime < 16667);

		// 終了命令が出ていたらループを抜ける
		if (!m_isRunning) break;
	}
}

void Application::Tarminate() const
{
	// Effekseerのあとしまつ
	Effkseer_End();

	// DxLibの終了処理
	DxLib_End();

#if _DEBUG//コンソールDebug用
	fclose(out); fclose(in); FreeConsole();//コンソール解放
#endif
}

void Application::QuitGame()
{
	m_isRunning = false;
}