#include "Application.h"
#include "EffectManager.h"
#include "Game.h"
#include "Input.h"
#include "MyRandom.h"
#include "SceneController.h"
#include "SoundManager.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>


namespace
{
#if _DEBUG
	// コンソール用
	FILE* in = 0;
	FILE* out = 0;
#endif

	constexpr LONGLONG kWaitTime = 16667;
}


Application::Application() :
	m_isRunning(true),
	m_isPause(false)
{
}

bool Application::DebugPause()
{
	// ポーズなら、ポーズボタンを押したら解除
	// また、コマ送りボタンで一回だけUpdateを実行

	// ポーズでなければ、ポーズボタンを押したら実行が止まる

	auto& input = Input::GetInstance();

	if (m_isPause)
	{
		if (input.IsTrigger("Pause"))
		{
			m_isPause = false;
			return false;
		}
		if (input.IsTrigger("FrameForward"))
		{
			return false;
		}

		return true;
	}
	else
	{
		if (input.IsTrigger("Pause"))
		{
			m_isPause = true;
			return true;
		}

		return false;
	}
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

	// 乱数を初期化
	MyRandom::GetInstance().Init();

	return true;
}

void Application::Run()
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

		input.Update();

#if _DEBUG
		// デバッグの一時停止処理
		if (DebugPause())
		{	
			// 60fpsになるように調整してる
			while (GetNowHiPerformanceCount() - beforFrameTime < 16667);

			continue;
		}
#endif

		// 画面をきれいに
		ClearDrawScreen();

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