#pragma once
#include "EnemyPlantState.h"

// ‘Ò‹@ó‘ÔˆÈã‚Ì‚±‚Æ‚ğ‚µ‚Ä‚é‚¯‚Ç‹–‚µ‚Ä‚Ë
class EnemyPlantIdle : public EnemyPlantState
{
public:
	EnemyPlantIdle(std::weak_ptr<EnemyPlant> parent);
	~EnemyPlantIdle();

	std::shared_ptr<EnemyPlantState> Update() override;

private:

	int m_attackFrame;
};

