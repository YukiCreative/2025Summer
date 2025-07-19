#pragma once
#include "EnemyEliteState.h"
class EnemyEliteIdle : public EnemyEliteState
{
public:
	EnemyEliteIdle(std::weak_ptr<EnemyElite> parent);
	~EnemyEliteIdle();

	std::shared_ptr<EnemyEliteState> Update() override;

private:

	int m_attackFrame;
};

