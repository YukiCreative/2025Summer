#include "EnemyBugIdle.h"
#include "EnemyBug.h"
#include "EnemyBugWalkFoward.h"
#include <random>
#include "EnemyBugAttack.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
	constexpr float kApproachDistance = 500.0f;

	constexpr int kAttackFrame = 50;
	constexpr int kRandomness = 10;
}

EnemyBugIdle::EnemyBugIdle(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent),
	m_frame(0),
	m_attackFrame(0)
{
	// アニメーション再生
	m_parent.lock()->ChangeAnim(kAnimName, true);

	std::random_device randomDevice;
	std::default_random_engine randEngine(randomDevice());
	auto dist = std::normal_distribution<>(kAttackFrame, kRandomness);

	m_attackFrame = dist(randEngine);
	printf("今回の攻撃猶予%d\n", m_attackFrame);
}

EnemyBugIdle::~EnemyBugIdle()
{
}

std::shared_ptr<EnemyBugState> EnemyBugIdle::Update()
{
	// プレイヤーを見る
	// 接近はしない 様子見みたいな
	auto parent = m_parent.lock();

	// 一定距離離れていたら、もしくはランダム時間経ったら
	if (parent->EnemyToPlayer().SqrMagnitude() > kApproachDistance * kApproachDistance)
	{
		return std::make_shared<EnemyBugWalkFoward>(m_parent);
	}

	if (m_frame == m_attackFrame)
	{
		return std::make_shared<EnemyBugAttack>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
