#pragma once
#include "EnemyPlantState.h"

class EnemyPlantKnockUp : public EnemyPlantState
{
public:
	EnemyPlantKnockUp(std::weak_ptr<EnemyPlant> parent);
	~EnemyPlantKnockUp();

	std::shared_ptr<EnemyPlantState> Update() override;
};

