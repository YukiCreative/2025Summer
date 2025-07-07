#include "EnemyBugIdle.h"
#include "EnemyBug.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
	//constexpr float kApproachDistance = 
}

EnemyBugIdle::EnemyBugIdle(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	// �A�j���[�V�����Đ�
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyBugIdle::~EnemyBugIdle()
{
}

std::shared_ptr<EnemyBugState> EnemyBugIdle::Update()
{
	// �v���C���[������
	// �ڋ߂͂��Ȃ� �l�q���݂�����
	auto parent = m_parent.lock();

	parent->LookAtPlayer();

	// ���ȏ㗣��Ă�����ڋ�
	//if (parent->EnemyToPlayer().SqrMagnitude() > k)

	return shared_from_this();
}
