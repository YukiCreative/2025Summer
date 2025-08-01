#include "WaveController.h"
#include "../Actor/Enemy/EnemyGenerator.h"
#include "../Actor/ActorController.h"

namespace
{
	constexpr int kMaxWave = 2;
	constexpr int kWaitFrame = 180;
}

WaveController::WaveController() :
	m_wave(0),
	m_stateFrame(0),
	m_state(&WaveController::IntervalUpdate),
	m_isDefeatedAllEnemy(false)
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
	m_isDefeatedAllEnemy = false;
	(this->*m_state)();
}

void WaveController::BattleUpdate()
{
	// 敵が全滅してるか監視
	// 全滅したらIntervalへ

	if (m_actors.lock()->SearchEnemy().size() == 0)
	{
		++m_wave;
		m_isDefeatedAllEnemy = true;
		if (m_wave > kMaxWave)
		{
			// 終了
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
	// 一定時間待つ
	// 待ったら敵を出現させてBattle

	if (m_stateFrame > kWaitFrame)
	{
		m_enemyGenerator->SpawnWave(m_wave);
		m_stateFrame = 0;
		m_state = &WaveController::BattleUpdate;
		return;
	}

	++m_stateFrame;
}
