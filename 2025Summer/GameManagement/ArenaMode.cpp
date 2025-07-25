#include "ArenaMode.h"
#include "WaveController.h"
#include "Timer.h"

namespace
{
	constexpr int kInitTimeLimit = 3600;
}

ArenaMode::ArenaMode()
{
}

void ArenaMode::Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController> actors)
{
	m_wave = std::make_shared<WaveController>();
	m_wave->Init(player, actors);

	m_timer = std::make_shared<Timer>();
	m_timer->Init();
	m_timer->SetCount(kInitTimeLimit);
	m_timer->SetStateCountDown();
	m_timer->StartCount();
}

void ArenaMode::Update()
{
	m_wave->Update();

	// ���Ԑ؂�̏���
	if (m_timer->Update())
	{
		m_wave->StopUpdate();

		// �V�[���J�ڂƂ�
	}
}