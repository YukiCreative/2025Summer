#include "WaveController.h"
#include "EnemyGenerator.h"
#include "ActorController.h"

namespace
{
	constexpr int kMaxWave = 2;
	constexpr int kWaitFrame = 180;
}

WaveController::WaveController() :
	m_wave(0),
	m_stateFrame(0),
	m_state(&WaveController::IntervalUpdate)
{
}

void WaveController::Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController> actors)
{
	m_actors = actors;

	m_enemyGenerator = std::make_shared<EnemyGenerator>();
	m_enemyGenerator->Init(player, m_actors);
}

void WaveController::Update()
{
	(this->*m_state)();
}

void WaveController::BattleUpdate()
{
	// “G‚ª‘S–Å‚µ‚Ä‚é‚©ŠÄŽ‹
	// ‘S–Å‚µ‚½‚çInterval‚Ö

	if (m_actors.lock()->SearchEnemy().size() == 0)
	{
		++m_wave;
		if (m_wave > kMaxWave)
		{
			// I—¹
			StopUpdate();
			return;
		}

		m_stateFrame = 0;
		m_state = &WaveController::IntervalUpdate;
		return;
	}

	++m_stateFrame;
}

void WaveController::IntervalUpdate()
{
	// ˆê’èŽžŠÔ‘Ò‚Â
	// ‘Ò‚Á‚½‚ç“G‚ðoŒ»‚³‚¹‚ÄBattle

	if (m_stateFrame > kWaitFrame)
	{
		m_enemyGenerator->SpawnWave(m_wave);
		m_stateFrame = 0;
		m_state = &WaveController::BattleUpdate;
		return;
	}

	++m_stateFrame;
}
