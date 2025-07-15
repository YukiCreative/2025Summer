#include "EnemyPlantAttack.h"
#include "EnemyPlant.h"
#include "EnemyPlantIdle.h"

namespace
{
	const std::string kAnimName = "Armature|Attack";

	constexpr int kAttackFrame = 30;
}

EnemyPlantAttack::EnemyPlantAttack(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// �A�j���[�V����
	m_parent.lock()->ChangeAnim(kAnimName, false);
}

EnemyPlantAttack::~EnemyPlantAttack()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantAttack::Update()
{
	auto parent = m_parent.lock();

	// ����̃t���[���ōU��
	// �e�𐶐�
	if (m_frame == kAttackFrame)
	{
		parent->SpawnBullet();
	}

	if (parent->IsEndAnim())
	{
		return std::make_shared<EnemyPlantIdle>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
