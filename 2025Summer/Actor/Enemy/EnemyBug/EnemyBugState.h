#pragma once
#include <memory>

class EnemyBug;

// �e�G���v���C���[�Ɠ����悤�ɏ�ԃN���X�ŊǗ�����
class EnemyBugState abstract : public std::enable_shared_from_this<EnemyBugState>
{
public:
	EnemyBugState(std::weak_ptr<EnemyBug> parent) : m_parent(parent) {}
	virtual ~EnemyBugState() {}

	virtual std::shared_ptr<EnemyBugState> Update() abstract;

protected:

	std::weak_ptr<EnemyBug> m_parent;

private:

	// ���ʂŎg�������֐��Ƃ�
};

