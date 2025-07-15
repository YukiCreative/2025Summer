#include "EnemyPlantIdle.h"
#include "EnemyPlant.h"
#include "EnemyPlantAttack.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
	constexpr float kRotateSpeed = 0.02f;
	constexpr float kEscapeDistance = 500.0f;
	constexpr float kEscapeSpeed = 0.2f;
}

EnemyPlantIdle::EnemyPlantIdle(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// Anim
	m_parent.lock()->ChangeAnim(kAnimName);

	// ����̍U���t���[�����擾
	m_attackFrame = m_parent.lock()->GetAttackInterval();
}

EnemyPlantIdle::~EnemyPlantIdle()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantIdle::Update()
{
	auto parent = m_parent.lock();

	// �v���C���[�̂ق�������
	// ������Ƒ��x�Ƀ���������
	parent->LookAtPlayer(fabsf(sinf(m_frame * kRotateSpeed)) * 0.5f + 0.1f);

	// �v���C���[����苗���ɋ߂Â����痣�������Ɉړ�
	if (parent->EnemyToPlayer().SqrMagnitude() < kEscapeDistance * kEscapeDistance)
	{
		parent->AddVel(-parent->EnemyToPlayer().GetNormalize() * kEscapeSpeed);
	}

	// �����_���ȊԊu�ōU��
	if (m_frame == m_attackFrame)
	{
		return std::make_shared<EnemyPlantAttack>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
