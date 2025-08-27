#include "EnemyPlantGetUp.h"
#include "EnemyPlant.h"
#include "EnemyPlantIdle.h"

namespace
{
	const std::string kStunAnimName = "Armature|GetUp";
}

EnemyPlantGetUp::EnemyPlantGetUp(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	m_parent.lock()->ChangeAnim(kStunAnimName, false);
	// スタン値を回復
	m_parent.lock()->RecoveryStun();
}

EnemyPlantGetUp::~EnemyPlantGetUp()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantGetUp::Update()
{
	// アニメーションが終わったら遷移
	if (m_parent.lock()->IsEndAnim())
	{
		return std::make_shared<EnemyPlantIdle>(m_parent);
	}


	return shared_from_this();
}
