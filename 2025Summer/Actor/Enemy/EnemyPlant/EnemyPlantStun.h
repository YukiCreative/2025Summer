#pragma once
#include "EnemyPlantState.h"

class EnemyPlantStun : public EnemyPlantState
{
public:
	EnemyPlantStun(std::weak_ptr<EnemyPlant> parent);
	~EnemyPlantStun();

	std::shared_ptr<EnemyPlantState> Update() override;

private:

};

