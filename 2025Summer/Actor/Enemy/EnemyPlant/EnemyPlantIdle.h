#pragma once
#include "EnemyPlantState.h"

// 待機状態以上のことをしてるけど許してね
class EnemyPlantIdle : public EnemyPlantState
{
public:
	EnemyPlantIdle(std::weak_ptr<EnemyPlant> parent);
	~EnemyPlantIdle();

	std::shared_ptr<EnemyPlantState> Update() override;

private:

	int m_attackFrame;
};

