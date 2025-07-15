#include "EnemyPlantIdle.h"
#include "EnemyPlant.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
}

EnemyPlantIdle::EnemyPlantIdle(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// Anim
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyPlantIdle::~EnemyPlantIdle()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantIdle::Update()
{
	// �v���C���[�̂ق�������

	// �v���C���[����苗���ɋ߂Â����痣�������Ɉړ�

	// �����_���ȊԊu�ōU��

	return shared_from_this();
}
