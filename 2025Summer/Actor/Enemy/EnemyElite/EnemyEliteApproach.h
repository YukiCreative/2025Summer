#pragma once
#include "EnemyEliteState.h"

class EnemyEliteApproach : public EnemyEliteState
{
public:
	EnemyEliteApproach(std::weak_ptr<EnemyElite> parent);
	~EnemyEliteApproach();

	std::shared_ptr<EnemyEliteState> Update() override;
};

