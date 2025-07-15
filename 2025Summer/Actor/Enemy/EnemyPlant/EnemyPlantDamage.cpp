#include "EnemyPlantDamage.h"
#include "EnemyPlant.h"

namespace
{
	const std::string kFrontAnim = "Armature|HitReactFoward";
	const std::string kBackAnim  = "Armature|HitReactBack";
	const std::string kRightAnim = "Armature|HitReactRight";
	const std::string kLeftAnim  = "Armature|HitReactLeft";
}

EnemyPlantDamage::EnemyPlantDamage(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// アニメーション
	m_parent.lock()->ChangeAnim();
}

EnemyPlantDamage::~EnemyPlantDamage()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantDamage::Update()
{
	// 終わるまで待機

	return shared_from_this();
}
