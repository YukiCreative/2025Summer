#pragma once
#include "EnemyBugState.h"

// �����]���̎��ɃA�j���[�V���������Ȃ��ƈ�a�����������̂ŁA��ԂƂ��ēƗ�������
class EnemyBugRotate : public EnemyBugState
{
public:
	EnemyBugRotate(std::weak_ptr<EnemyBug> parent);
	~EnemyBugRotate();

	std::shared_ptr<EnemyBugState> Update() override;
};

