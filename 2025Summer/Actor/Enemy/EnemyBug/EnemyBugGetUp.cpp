#include "EnemyBugGetUp.h"
#include "EnemyBug.h"
#include "EnemyBugIdle.h"

namespace
{
	const std::string kStunAnimName = "Armature|GetUp";
}

EnemyBugGetUp::EnemyBugGetUp(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	m_parent.lock()->ChangeAnim(kStunAnimName, false);
	// スタン値を回復
	m_parent.lock()->RecoveryStun();
}

EnemyBugGetUp::~EnemyBugGetUp()
{
}

std::shared_ptr<EnemyBugState> EnemyBugGetUp::Update()
{
	// アニメーションが終わったら遷移
	if (m_parent.lock()->IsEndAnim())
	{
		return std::make_shared<EnemyBugIdle>(m_parent);
	}

	return shared_from_this();
}
