#include "EnemyPlantStun.h"
#include "EnemyPlant.h"
#include "EnemyPlantKnockUp.h"
#include "EnemyPlantIdle.h"

namespace
{
	const std::string kStunAnimName = "Armature|Stun";
	const std::string kDamageAnimName = "Armature|HitReactBack";
	constexpr int kStunFrame = 300;
}

EnemyPlantStun::EnemyPlantStun(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	m_parent.lock()->ChangeAnim(kStunAnimName);
}

EnemyPlantStun::~EnemyPlantStun()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantStun::Update()
{
	std::shared_ptr<EnemyPlant> parent = m_parent.lock();
	// ダメージを受けたらダメージアニメーションを流す
	if (parent->IsDamagedInThisFrame())
	{
		parent->ChangeAnim(kDamageAnimName, false);
	}

	if (parent->CompareAnim(kDamageAnimName) && parent->IsEndAnim())
	{
		parent->ChangeAnim(kStunAnimName);
	}

	// 打ち上げ攻撃を受けたら打ち上げ状態へ
	if (parent->IsKnockUp())
	{
		return std::make_shared<EnemyPlantKnockUp>(m_parent);
	}

	// 一定時間経過したら通常状態へ
	if (m_frame > kStunFrame)
	{
		parent->RecoveryStun();
		return std::make_shared<EnemyPlantIdle>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
