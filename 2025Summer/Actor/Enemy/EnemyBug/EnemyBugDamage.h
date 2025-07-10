#pragma once
#include "EnemyBugState.h"

class EnemyBugDamage : public EnemyBugState
{
public:
	EnemyBugDamage(std::weak_ptr<EnemyBug> parent);
	~EnemyBugDamage();

	std::shared_ptr<EnemyBugState> Update() override;
};

