#pragma once
#include "EnemyPlantState.h"
class EnemyPlantDamage : public EnemyPlantState
{
public:
	EnemyPlantDamage(std::weak_ptr<EnemyPlant> parent);
	~EnemyPlantDamage();

	std::shared_ptr<EnemyPlantState> Update() override;
};

