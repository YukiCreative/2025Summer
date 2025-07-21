#pragma once
#include "EnemyEliteState.h"

class EnemyEliteArmSwing : public EnemyEliteState
{
public:
	EnemyEliteArmSwing(std::weak_ptr<EnemyElite> parent);
	~EnemyEliteArmSwing();

	std::shared_ptr<EnemyEliteState> Update() override;
};

