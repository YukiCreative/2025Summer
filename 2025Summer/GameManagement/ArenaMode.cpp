#include "ArenaMode.h"
#include "WaveController.h"
#include "Timer.h"
#include "Game.h"
#include "UIController.h"
#include "TimeUI.h"
#include "SceneController.h"
#include "SceneResult.h"
#include "ActorController.h"

#include "Actor.h"

namespace
{
	constexpr int kInitTimeLimit = 60 * Game::kFrameRate;
	const Vector2 kTimerPos = {Game::kScreenHalfWidth, 50};
}

ArenaMode::ArenaMode()
{
	m_addFrameMap[EnemyKind::kNone ] =   0;
	m_addFrameMap[EnemyKind::kBug  ] = 300;
	m_addFrameMap[EnemyKind::kPlant] = 300;
	m_addFrameMap[EnemyKind::kElite] = 600;
}

void ArenaMode::Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController> actors, std::weak_ptr<UIController> ui)
{
	m_actors = actors;
	m_wave = std::make_shared<WaveController>();
	m_wave->Init(player, actors);

	m_timer = std::make_shared<Timer>();
	m_timer->Init();
	m_timer->SetCount(kInitTimeLimit);
	m_timer->SetStateCountDown();
	m_timer->StartCount();

	auto timer = std::make_shared<TimeUI>();
	timer->Init(kTimerPos, m_timer);
	ui.lock()->AddUI(timer);
}

void ArenaMode::Update()
{
	m_wave->Update();

	// 時間切れの処理
	if (m_timer->Update())
	{
		m_timer->SetCount(0);
		m_timer->StopCount();
		m_wave->StopUpdate();

		// シーン遷移とか
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneResult>());
		return;
	}

	// 敵が死んだら
	auto deathEnemyList = m_actors.lock()->GetBeforeDeathEnemy();
	for (auto& enemy : deathEnemyList)
	{
		// タイマー延長
		m_timer->AddCount(m_addFrameMap[enemy]);
	}
}