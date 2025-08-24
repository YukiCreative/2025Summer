#pragma once
#include "EnemyBugState.h"

// UŒ‚ˆêí—Ş‚Å‚¢‚¢‚æ‚ÈH
class EnemyBugAttack : public EnemyBugState
{
public:
	EnemyBugAttack(std::weak_ptr<EnemyBug> parent);
	~EnemyBugAttack();

	std::shared_ptr<EnemyBugState> Update() override;
private:
};