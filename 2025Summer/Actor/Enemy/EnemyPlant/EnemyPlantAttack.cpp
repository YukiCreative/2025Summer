#include "EnemyPlantAttack.h"

namespace
{
	const std::string kAnimName = "";
}

EnemyPlantAttack::EnemyPlantAttack(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// アニメーション
}

EnemyPlantAttack::~EnemyPlantAttack()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantAttack::Update()
{
	// 特定のフレームで攻撃
	// 弾を生成

	return shared_from_this();
}
