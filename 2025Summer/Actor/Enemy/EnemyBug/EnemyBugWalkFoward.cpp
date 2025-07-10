#include "EnemyBugWalkFoward.h"
#include "EnemyBug.h"
#include "EnemyBugIdle.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|Walk";
	constexpr float kWalkSpeed = 0.5f;
	constexpr float kStopWalkDistance = 200.0f;
}

EnemyBugWalkFoward::EnemyBugWalkFoward(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	// アニメーション
	m_parent.lock()->ChangeAnim(kAnimName, true);
}

EnemyBugWalkFoward::~EnemyBugWalkFoward()
{
}

std::shared_ptr<EnemyBugState> EnemyBugWalkFoward::Update()
{
	// 接近
	auto parent = m_parent.lock();

	parent->LookAtPlayer();

	// 自分の向きに前進
	parent->AddVel(VTransformSR({0,0,kWalkSpeed}, parent->GetModelMatrix()));

	// 一定距離に近づいたら歩くのをやめる
	if (parent->EnemyToPlayer().SqrMagnitude() < kStopWalkDistance * kStopWalkDistance)
	{
		return std::make_shared<EnemyBugIdle>(m_parent);
	}

	return shared_from_this();
}
