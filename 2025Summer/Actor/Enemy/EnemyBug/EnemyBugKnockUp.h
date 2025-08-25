#pragma once
#include "EnemyBugState.h"

class EnemyBugKnockUp : public EnemyBugState
{
public:
	EnemyBugKnockUp(std::weak_ptr<EnemyBug> parent);
	~EnemyBugKnockUp();

	std::shared_ptr<EnemyBugState> Update() override;

private:
};

