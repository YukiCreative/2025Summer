#include "EnemyBugKnockUp.h"
#include "EnemyBug.h"
#include "EnemyBugGetUp.h"

namespace
{
	const std::string kAnimName = "Armature|DownBack";
}

EnemyBugKnockUp::EnemyBugKnockUp(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	m_parent.lock()->ChangeAnim(kAnimName, false);
}

EnemyBugKnockUp::~EnemyBugKnockUp()
{
}

std::shared_ptr<EnemyBugState> EnemyBugKnockUp::Update()
{
	// 地面についたら起き上がりモーションを流す
	if (!m_parent.lock()->IsKnockUp())
	{
		return std::make_shared<EnemyBugGetUp>(m_parent);
	}

	return shared_from_this();
}
