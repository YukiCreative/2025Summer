#pragma once
#include "EnemyBugState.h"

// �ҋ@
class EnemyBugIdle : public EnemyBugState
{
public:
	EnemyBugIdle(std::weak_ptr<EnemyBug> parent);
	~EnemyBugIdle();

	std::shared_ptr<EnemyBugState> Update() override;
};

