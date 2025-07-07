#include "EnemyBugRotate.h"
#include "EnemyBug.h"

namespace
{
	const std::string kAnimName = "Armature|Walk";
}

EnemyBugRotate::EnemyBugRotate(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	// �����A�j���[�V�����ŉ�]�������Ɍ�����
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyBugRotate::~EnemyBugRotate()
{
}

std::shared_ptr<EnemyBugState> EnemyBugRotate::Update()
{
	// ��]
	m_parent.lock()->LookAtPlayer();

	// ����

	return shared_from_this();
}

