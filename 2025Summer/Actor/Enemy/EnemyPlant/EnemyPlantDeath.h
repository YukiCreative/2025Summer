#pragma once
#include "EnemyPlantState.h"
class EnemyPlantDeath : public EnemyPlantState
{
public:
	EnemyPlantDeath(std::weak_ptr<EnemyPlant> parent);
	~EnemyPlantDeath();

	std::shared_ptr<EnemyPlantState> Update() override;
};

