#include "EnemyPlantAttack.h"

namespace
{
	const std::string kAnimName = "";
}

EnemyPlantAttack::EnemyPlantAttack(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// �A�j���[�V����
}

EnemyPlantAttack::~EnemyPlantAttack()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantAttack::Update()
{
	// ����̃t���[���ōU��
	// �e�𐶐�

	return shared_from_this();
}
