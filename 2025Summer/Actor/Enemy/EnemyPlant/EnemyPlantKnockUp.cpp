#include "EnemyPlantKnockUp.h"
#include "EnemyPlant.h"
#include "EnemyPlantGetUp.h"

namespace
{
	const std::string kAnimName = "Armature|KnockUp";
}

EnemyPlantKnockUp::EnemyPlantKnockUp(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	m_parent.lock()->ChangeAnim(kAnimName, false);
}

EnemyPlantKnockUp::~EnemyPlantKnockUp()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantKnockUp::Update()
{
	// 地面についたら起き上がりモーションを流す
	// この辺の処理を共通化したかった
	if (!m_parent.lock()->IsKnockUp())
	{
		return std::make_shared<EnemyPlantGetUp>(m_parent);
	}

	m_parent.lock()->KnockUpUpdate();

	return shared_from_this();
}
