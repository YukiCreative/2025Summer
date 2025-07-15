#include "EnemyPlantDeath.h"

EnemyPlantDeath::EnemyPlantDeath(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// アニメーション


	// 無敵化
}

EnemyPlantDeath::~EnemyPlantDeath()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantDeath::Update()
{
	// アニメーションが終わるまで待機

	return shared_from_this();
}
