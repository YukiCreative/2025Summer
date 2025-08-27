#include "EnemyEliteKnockUp.h"
#include "EnemyElite.h"
#include "EnemyEliteGetUp.h"

namespace
{
	const std::string kAnimName = "Armature|KnockUp";
}

EnemyEliteKnockUp::EnemyEliteKnockUp(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kAnimName, false);
}

EnemyEliteKnockUp::~EnemyEliteKnockUp()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteKnockUp::Update()
{
	// 地面についたら起き上がりモーションを流す
	if (!m_parent.lock()->IsKnockUp())
	{
		return std::make_shared<EnemyEliteGetUp>(m_parent);
	}

	m_parent.lock()->KnockUpUpdate();

	return shared_from_this();
}
