#include "EnemyBugIdle.h"
#include "EnemyBug.h"
#include "EnemyBugWalkFoward.h"
#include <random>
#include "EnemyBugAttack.h"
#include "Geometry.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
	constexpr float kApproachDistance = 600.0f;

	constexpr float kRotateThreshold = DX_PI_F * 0.25f;
}

EnemyBugIdle::EnemyBugIdle(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent),
	m_frame(0)
{
	// �A�j���[�V�����Đ�
	m_parent.lock()->ChangeAnim(kAnimName, true);
}

EnemyBugIdle::~EnemyBugIdle()
{
}

std::shared_ptr<EnemyBugState> EnemyBugIdle::Update()
{
	// �v���C���[������
	// �ڋ߂͂��Ȃ� �l�q���݂�����
	auto parent = m_parent.lock();

	const bool isDistancePlayer = parent->EnemyToPlayer().SqrMagnitude() > kApproachDistance * kApproachDistance;
	const bool isRotatePlayer = Geometry::Corner(parent->EnemyToPlayer().XZ(), parent->GetDir().XZ()) > kRotateThreshold;

	// ��苗������Ă�����A�������̓v���C���[�������̌����Ƃ낤�Ƃ�����
	if (isDistancePlayer || isRotatePlayer)
	{
		return std::make_shared<EnemyBugWalkFoward>(m_parent);
	}

	// ���̏�Ԃ������Ă���Ȃ�U���t���[����i�߂�
	parent->CountAttackFrame();

	if (parent->GetAttackFrame() < 0)
	{
		return std::make_shared<EnemyBugAttack>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
