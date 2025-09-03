#include "EnemyEliteDeath.h"
#include "EnemyElite.h"

namespace
{
	const std::string kStunAnimName = "Armature|DownBack";
	constexpr int kDeathTime = 120;
}

EnemyEliteDeath::EnemyEliteDeath(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kStunAnimName, false);
	m_parent.lock()->SetInvincibility(true);
	m_parent.lock()->StartBloodEffect();
	m_parent.lock()->SetCanLockOn(false);

	m_parent.lock()->ShotDeathSound();
}

EnemyEliteDeath::~EnemyEliteDeath()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteDeath::Update()
{
	if (m_frame > kDeathTime)
	{
		m_parent.lock()->OnDeath();
	}

	++m_frame;

	return shared_from_this();
}
