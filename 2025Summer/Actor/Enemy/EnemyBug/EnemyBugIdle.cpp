#include "EnemyBugIdle.h"
#include "EnemyBug.h"
#include "EnemyBugWalkFoward.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
	constexpr float kApproachDistance = 1000.0f;
}

EnemyBugIdle::EnemyBugIdle(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	// アニメーション再生
	m_parent.lock()->ChangeAnim(kAnimName);
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
	if (parent->EnemyToPlayer().SqrMagnitude() > kApproachDistance * kApproachDistance);
	{
		return std::make_shared<EnemyBugWalkFoward>(m_parent);
	}

	return shared_from_this();
}
