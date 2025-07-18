#include "EnemyEliteIdle.h"
#include "EnemyElite.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
}

EnemyEliteIdle::EnemyEliteIdle(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyEliteIdle::~EnemyEliteIdle()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteIdle::Update()
{
	return shared_from_this();
}
