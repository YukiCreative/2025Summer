#include "EnemyEliteDeath.h"
#include "EnemyElite.h"

namespace
{
	const std::string kAnimName = "Armature|DownBack";
}

EnemyEliteDeath::EnemyEliteDeath(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyEliteDeath::~EnemyEliteDeath()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteDeath::Update()
{
	if (m_parent.lock()->IsEndAnim())
	{
		m_parent.lock()->OnDeath();
	}

	++m_frame;

	return shared_from_this();
}
