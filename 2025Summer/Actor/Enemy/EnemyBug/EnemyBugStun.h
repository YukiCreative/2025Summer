#pragma once
#include "EnemyBugState.h"

// ƒXƒ^ƒ“ó‘Ô
class EnemyBugStun : public EnemyBugState
{
public:
	EnemyBugStun(std::weak_ptr<EnemyBug> parent);
	~EnemyBugStun();

	std::shared_ptr<EnemyBugState> Update() override;

private:
};

