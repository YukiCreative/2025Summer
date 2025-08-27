#pragma once
#include "EnemyPlantState.h"

class EnemyPlantGetUp : public EnemyPlantState
{
public:
	EnemyPlantGetUp(std::weak_ptr<EnemyPlant> parent);
	~EnemyPlantGetUp();

	std::shared_ptr<EnemyPlantState> Update() override;

private:
};

