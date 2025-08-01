#include "ArenaMode.h"
#include "WaveController.h"
#include "Timer.h"
#include "../General/Game.h"
#include "../UI/UIController.h"
#include "../UI/TimeUI.h"
#include "../Scene/SceneController.h"
#include "../Scene/SceneResult.h"
#include "../Actor/ActorController.h"
#include "../Actor/Enemy/EnemyGenerator.h"

#include "../Actor/Actor.h"

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

	m_enemyGenerator = std::make_shared<EnemyGenerator>();
	m_enemyGenerator->Init(player, actors);
}

void ArenaMode::Update()
{
	m_wave->Update();

	// ���Ԑ؂�̏���
	if (m_timer->Update())
	{
		m_timer->SetCount(0);
		m_timer->StopCount();
		//m_wave->StopUpdate();

		// �V�[���J�ڂƂ�
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneResult>());
		return;
	}

	// �G�����񂾂�
	auto deathEnemyList = m_actors.lock()->GetBeforeDeathEnemy();
	for (auto& enemy : deathEnemyList)
	{
		// �^�C�}�[����
		m_timer->AddCount(m_addFrameMap[enemy]);
	}

	// �E�F�[�u�ł������G���S�����񂾂�
	// UI���o��
	// �X�R�A�o��
	if (m_wave->IsDefeatedAllEnemy())
	{

	}
}