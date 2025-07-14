#pragma once
#include "EnemyBugState.h"
class EnemyBugWalkFoward : public EnemyBugState
{
public:
	EnemyBugWalkFoward(std::weak_ptr<EnemyBug> parent);
	~EnemyBugWalkFoward();

	std::shared_ptr<EnemyBugState> Update() override;
private:

	int m_rotateFrame;
	int m_rotateTimeLength;
};

