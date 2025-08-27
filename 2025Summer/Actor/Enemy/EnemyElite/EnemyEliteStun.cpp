#include "EnemyEliteStun.h"
#include "EnemyElite.h"
#include "EnemyEliteKnockUp.h"
#include "EnemyEliteIdle.h"

namespace
{
	const std::string kStunAnimName = "Armature|Stun";
	const std::string kDamageAnimName = "Armature|KnockBack";
	constexpr int kStunFrame = 300;
}

EnemyEliteStun::EnemyEliteStun(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kStunAnimName);
}

EnemyEliteStun::~EnemyEliteStun()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteStun::Update()
{
	std::shared_ptr<EnemyElite> parent = m_parent.lock();
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
		return std::make_shared<EnemyEliteKnockUp>(m_parent);
	}

	// 一定時間経過したら通常状態へ
	if (m_frame > kStunFrame)
	{
		parent->RecoveryStun();
		return std::make_shared<EnemyEliteIdle>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
