#include "EnemyEliteGetUp.h"
#include "EnemyElite.h"
#include "EnemyEliteIdle.h"

namespace
{
	const std::string kStunAnimName = "Armature|GetUp";
}

EnemyEliteGetUp::EnemyEliteGetUp(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kStunAnimName, false);
	// スタン値を回復
	m_parent.lock()->RecoveryStun();
}

EnemyEliteGetUp::~EnemyEliteGetUp()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteGetUp::Update()
{
	// アニメーションが終わったら遷移
	if (m_parent.lock()->IsEndAnim())
	{
		return std::make_shared<EnemyEliteIdle>(m_parent);
	}

	return shared_from_this();
}
