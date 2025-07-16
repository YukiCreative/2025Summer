#include "EnemyBugIdle.h"
#include "EnemyBug.h"
#include "EnemyBugWalkFoward.h"
#include <random>
#include "EnemyBugAttack.h"
#include "Geometry.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
	constexpr float kApproachDistance = 600.0f;

	constexpr float kRotateThreshold = DX_PI_F * 0.25f;
}

EnemyBugIdle::EnemyBugIdle(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent),
	m_frame(0)
{
	// アニメーション再生
	m_parent.lock()->ChangeAnim(kAnimName, true);
}

EnemyBugIdle::~EnemyBugIdle()
{
}

std::shared_ptr<EnemyBugState> EnemyBugIdle::Update()
{
	// プレイヤーを見る
	// 接近はしない 様子見みたいな
	auto parent = m_parent.lock();

	const bool isDistancePlayer = parent->EnemyToPlayer().SqrMagnitude() > kApproachDistance * kApproachDistance;
	const bool isRotatePlayer = Geometry::Corner(parent->EnemyToPlayer().XZ(), parent->GetDir().XZ()) > kRotateThreshold;

	// 一定距離離れていたら、もしくはプレイヤーが自分の後ろをとろうとしたら
	if (isDistancePlayer || isRotatePlayer)
	{
		return std::make_shared<EnemyBugWalkFoward>(m_parent);
	}

	// この状態が続いているなら攻撃フレームを進める
	parent->CountAttackFrame();

	if (parent->GetAttackFrame() < 0)
	{
		return std::make_shared<EnemyBugAttack>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
