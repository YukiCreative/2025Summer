#include "EnemyEliteBite.h"
#include "EnemyElite.h"
#include "EnemyEliteIdle.h"

namespace
{
	const std::string kStartAnimName = "Armature|Found";
	const std::string kAttackAnimName = "Armature|Bite";

	constexpr int kAttackFrame = 50;
}

EnemyEliteBite::EnemyEliteBite(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kStartAnimName, false);
}

EnemyEliteBite::~EnemyEliteBite()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteBite::Update()
{
	auto parent = m_parent.lock();

	if (parent->IsEndAnim())
	{
		if (parent->CheckAnimName(kStartAnimName))
		{
			parent->ChangeAnim(kAttackAnimName, false);
		}
		else
		{
			return std::make_shared<EnemyEliteIdle>(m_parent);
		}
	}

	if (m_frame == kAttackFrame)
	{
		parent->BiteAttack();
	}

	++m_frame;

	return shared_from_this();
}
