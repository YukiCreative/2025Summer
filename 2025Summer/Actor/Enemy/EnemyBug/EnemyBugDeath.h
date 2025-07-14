#pragma once
#include "EnemyBugState.h"

// óéñΩÅI
class EnemyBugDeath : public EnemyBugState
{
public:
	EnemyBugDeath(std::weak_ptr<EnemyBug> parent);
	~EnemyBugDeath();

	std::shared_ptr<EnemyBugState> Update() override;

private:

	int m_frame;
	int m_animTotalFrame;
};

