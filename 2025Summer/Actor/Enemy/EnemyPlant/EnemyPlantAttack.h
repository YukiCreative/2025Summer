#pragma once
#include "EnemyPlantState.h"

class EnemyPlantAttack : public EnemyPlantState
{
public:
	EnemyPlantAttack(std::weak_ptr<EnemyPlant> parent);
	~EnemyPlantAttack();

	std::shared_ptr<EnemyPlantState> Update() override;
};

