#include "EnemyPlantIdle.h"
#include "EnemyPlant.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
}

EnemyPlantIdle::EnemyPlantIdle(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// Anim
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyPlantIdle::~EnemyPlantIdle()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantIdle::Update()
{
	// プレイヤーのほうを向く

	// プレイヤーが一定距離に近づいたら離れる方向に移動

	// ランダムな間隔で攻撃

	return shared_from_this();
}
