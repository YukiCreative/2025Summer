#include "EnemyBugWalkFoward.h"
#include "EnemyBug.h"
#include "EnemyBugIdle.h"
#include <DxLib.h>
#include "Geometry.h"
#include "EnemyBugAttack.h"

namespace
{
	const std::string kAnimName = "Armature|Walk";
	constexpr float kWalkSpeed = 0.5f;
	constexpr float kStopWalkDistance = 200.0f;
	constexpr float kRotateThreshold = DX_PI_F * 0.1f;
}

EnemyBugWalkFoward::EnemyBugWalkFoward(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent),
	m_rotateFrame(0),
	m_rotateTimeLength(0)
{
	// �A�j���[�V����
	m_parent.lock()->ChangeAnim(kAnimName, true);
}

EnemyBugWalkFoward::~EnemyBugWalkFoward()
{
}

std::shared_ptr<EnemyBugState> EnemyBugWalkFoward::Update()
{
	// �ڋ�
	auto parent = m_parent.lock();

	// �����_���Ԋu�Ńv���C���[�̂ق�������
	// �����ƌ����Ă���ƕs���R�Ȃ̂�
	if (m_rotateFrame == 0)
	{
		// ��]���Ԃ���
		m_rotateTimeLength = 40;
		// ���̉�]�񕜎��Ԃ�ݒ�
		m_rotateFrame = GetRand(60) + m_rotateTimeLength;
	}
	if (m_rotateTimeLength > 0)
	{
		parent->LookAtPlayer();
	}

	--m_rotateFrame;
	--m_rotateTimeLength;

	const bool isClosePlayer = parent->EnemyToPlayer().SqrMagnitude() < kStopWalkDistance * kStopWalkDistance;
	const bool canHitAttackRotate = Geometry::Corner(parent->EnemyToPlayer().XZ(), parent->GetDir().XZ()) < kRotateThreshold;

	// �߂Â������Ɉړ��ł��āA�������Ȃ玩���̌����ɑO�i
	if (!isClosePlayer)
	{
		if (canHitAttackRotate)
		{
			parent->AddVel(VTransformSR({0,0,kWalkSpeed}, parent->GetModelMatrix()));
		}
	}
	else
	{
		// �\���ɋ߂Â��Ă�����U���^�C�}�[��i�߂�
		parent->CountAttackFrame();
	}

	// ��苗���ɋ߂Â���&��������������Ȃ�����̂���߂�
	if (isClosePlayer && canHitAttackRotate)
	{
		return std::make_shared<EnemyBugIdle>(m_parent);
	}

	// �U���J��
	if (parent->GetAttackFrame() < 0)
	{
		return std::make_shared<EnemyBugAttack>(m_parent);
	}

	return shared_from_this();
}
