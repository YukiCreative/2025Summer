#pragma once
#include "EnemyBugState.h"

// 方向転換の時にアニメーションをしないと違和感があったので、状態として独立させる
class EnemyBugRotate : public EnemyBugState
{
public:
	EnemyBugRotate(std::weak_ptr<EnemyBug> parent);
	~EnemyBugRotate();

	std::shared_ptr<EnemyBugState> Update() override;
};

