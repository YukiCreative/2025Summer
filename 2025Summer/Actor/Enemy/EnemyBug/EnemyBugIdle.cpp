#include "EnemyBugIdle.h"
#include "EnemyBug.h"
#include "EnemyBugWalkFoward.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
	constexpr float kApproachDistance = 1000.0f;
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

	// ��苗������Ă�����A�������̓����_�����Ԍo������
	if (parent->EnemyToPlayer().SqrMagnitude() > kApproachDistance * kApproachDistance);
	{
		return std::make_shared<EnemyBugWalkFoward>(m_parent);
	}

	return shared_from_this();
}
