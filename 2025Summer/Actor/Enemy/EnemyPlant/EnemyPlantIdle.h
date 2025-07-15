#pragma once
#include "EnemyPlantState.h"

// �ҋ@��Ԉȏ�̂��Ƃ����Ă邯�ǋ����Ă�
class EnemyPlantIdle : public EnemyPlantState
{
public:
	EnemyPlantIdle(std::weak_ptr<EnemyPlant> parent);
	~EnemyPlantIdle();

	std::shared_ptr<EnemyPlantState> Update() override;

private:

	int m_attackFrame;
};

