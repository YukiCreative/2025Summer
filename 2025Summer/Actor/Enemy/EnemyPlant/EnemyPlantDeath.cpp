#include "EnemyPlantDeath.h"

EnemyPlantDeath::EnemyPlantDeath(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// �A�j���[�V����


	// ���G��
}

EnemyPlantDeath::~EnemyPlantDeath()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantDeath::Update()
{
	// �A�j���[�V�������I���܂őҋ@

	return shared_from_this();
}
