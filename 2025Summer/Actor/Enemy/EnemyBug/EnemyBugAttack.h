#pragma once
#include "EnemyBugState.h"

// 攻撃一種類でいいよな？
class EnemyBugAttack : public EnemyBugState
{
public:
	EnemyBugAttack(std::weak_ptr<EnemyBug> parent);
	~EnemyBugAttack();

	std::shared_ptr<EnemyBugState> Update() override;
private:
	int m_frame;
};