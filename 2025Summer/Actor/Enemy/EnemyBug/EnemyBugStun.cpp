#include "EnemyBugStun.h"
#include "EnemyBug.h"
#include <string>
#include "EnemyBugIdle.h"
#include "EnemyBugKnockUp.h"

namespace
{
	const std::string kStunAnimName = "Armature|Stun";
	const std::string kDamageAnimName = "Armature|HitReact";
	constexpr int kStunFrame = 300;
}

EnemyBugStun::EnemyBugStun(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	// スタン中アニメーションを流したい
	m_parent.lock()->ChangeAnim(kStunAnimName);
}

EnemyBugStun::~EnemyBugStun()
{
}

std::shared_ptr<EnemyBugState> EnemyBugStun::Update()
{
	std::shared_ptr<EnemyBug> parent = m_parent.lock();
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
		return std::make_shared<EnemyBugKnockUp>(m_parent);
	}

	// 一定時間経過したら通常状態へ
	if (m_frame > kStunFrame)
	{
		parent->RecoveryStun();
		return std::make_shared<EnemyBugIdle>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
