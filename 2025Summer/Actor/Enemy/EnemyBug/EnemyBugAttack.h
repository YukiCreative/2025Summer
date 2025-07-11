#pragma once
#include "EnemyBugState.h"

// �U�����ނł�����ȁH
class EnemyBugAttack : public EnemyBugState
{
public:
	EnemyBugAttack(std::weak_ptr<EnemyBug> parent);
	~EnemyBugAttack();

	std::shared_ptr<EnemyBugState> Update() override;
private:
	int m_frame;
};