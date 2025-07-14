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
	// �R���\�[���p
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
	// �|�[�Y�Ȃ�A�|�[�Y�{�^���������������
	// �܂��A�R�}����{�^���ň�񂾂�Update�����s

	// �|�[�Y�łȂ���΁A�|�[�Y�{�^��������������s���~�܂�

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
	/*�R���\�[��Debug�p*/
#if _DEBUG
	AllocConsole();							// �R���\�[��
	freopen_s(&out, "CON", "w", stdout);	// stdout
	freopen_s(&in, "CON", "r", stdin);		// stdin
#endif

	// �������O�ɕK�v�ȏ��̒�`
	ChangeWindowMode(Game::kInitWindowMode);
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitDepth);

	// DxLib�̏������ŁA�G�������瑦�ُ�l��Ԃ�
	if (DxLib_Init() == -1) return false;

	// ����ʂɕ`�悵�܂�
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	// ������������
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

	// �Q�[�����[�v
	while (ProcessMessage() == 0)
	{
		// �J�n���̃R���s���[�^�̎��Ԃ��擾
		long long beforFrameTime = GetNowHiPerformanceCount();

		input.Update();

#if _DEBUG
		// �f�o�b�O�̈ꎞ��~����
		if (DebugPause())
		{	
			// 60fps�ɂȂ�悤�ɒ������Ă�
			while (GetNowHiPerformanceCount() - beforFrameTime < 16667);

			continue;
		}
#endif

		// ��ʂ����ꂢ��
		ClearDrawScreen();

		// �����ɃQ�[���̏���������
		scenes.Update();
		scenes.Draw();

		effect.Update();
		effect.Draw();

		sound.Update();

		// �`�悵������ʂ𔽉f
		ScreenFlip();

		// 60fps�ɂȂ�悤�ɒ������Ă�
		while (GetNowHiPerformanceCount() - beforFrameTime < 16667);

		// �I�����߂��o�Ă����烋�[�v�𔲂���
		if (!m_isRunning) break;
	}
}

void Application::Tarminate() const
{
	// Effekseer�̂��Ƃ��܂�
	Effkseer_End();

	// DxLib�̏I������
	DxLib_End();

#if _DEBUG//�R���\�[��Debug�p
	fclose(out); fclose(in); FreeConsole();//�R���\�[�����
#endif
}

void Application::QuitGame()
{
	m_isRunning = false;
}