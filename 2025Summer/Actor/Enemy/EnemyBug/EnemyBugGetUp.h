#pragma once
#include "EnemyBugState.h"

class EnemyBugGetUp : public EnemyBugState
{
public:
	EnemyBugGetUp(std::weak_ptr<EnemyBug> parent);
	~EnemyBugGetUp();

	std::shared_ptr<EnemyBugState> Update() override;

private:
};

