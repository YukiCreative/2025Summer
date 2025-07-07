#pragma once
#include "EnemyBugState.h"

// ‘Ò‹@
class EnemyBugIdle : public EnemyBugState
{
public:
	EnemyBugIdle(std::weak_ptr<EnemyBug> parent);
	~EnemyBugIdle();

	std::shared_ptr<EnemyBugState> Update() override;
};

