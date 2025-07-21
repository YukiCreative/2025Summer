#include "EnemyEliteIdle.h"
#include "EnemyElite.h"
#include "EnemyEliteApproach.h"
#include "EnemyEliteBite.h"
#include "EnemyEliteArmSwing.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|Idle";
	constexpr float kRotateSpeed = 0.01f;
	constexpr float kMoveDistance = 550.0f;
}

EnemyEliteIdle::EnemyEliteIdle(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kAnimName);
	m_attackFrame = m_parent.lock()->GetAttackInterval();
}

EnemyEliteIdle::~EnemyEliteIdle()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteIdle::Update()
{
	auto parent = m_parent.lock();

	// �����_���ȊԊu�ōU��
	if (m_frame > m_attackFrame)
	{
		// �r�U�艺�낵�U���͂��܂ɂ������Ȃ�
		if (GetRand(3))
		{
			return std::make_shared<EnemyEliteBite>(m_parent);
		}
		else
		{
			return std::make_shared<EnemyEliteArmSwing>(m_parent);
		}
	}

	if (parent->EnemyToPlayer().SqrMagnitude() > kMoveDistance * kMoveDistance)
	{
		return std::make_shared<EnemyEliteApproach>(m_parent);
	}

	// �v���C���[�̂ق�������
	// ������Ƒ��x�Ƀ���������
	parent->LookAtPlayer(fabsf(sinf(m_frame * kRotateSpeed)) * 0.5f);

	++m_frame;

	return shared_from_this();
}
