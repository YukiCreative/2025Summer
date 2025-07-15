#include "EnemyPlantDamage.h"
#include "EnemyPlant.h"
#include <array>
#include <DxLib.h>
#include "EnemyPlantIdle.h"

namespace
{
	const std::string kFrontAnim = "Armature|HitReactFront";
	const std::string kBackAnim  = "Armature|HitReactBack";
	const std::string kRightAnim = "Armature|HitReactRight";
	const std::string kLeftAnim  = "Armature|HitReactLeft";

	const std::array<std::string, 4> kAnimNames =
	{
		kFrontAnim,
		kBackAnim,
		kRightAnim,
		kLeftAnim
	};
}

EnemyPlantDamage::EnemyPlantDamage(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// アニメーション
	// せっかくだからランダムにリアクションさせよう
	int num = GetRand(3);
	m_parent.lock()->ChangeAnim(kAnimNames[num], false);
	printf("%d\n", num);
}

EnemyPlantDamage::~EnemyPlantDamage()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantDamage::Update()
{
	// 終わるまで待機
	if (m_parent.lock()->IsEndAnim())
	{
		return std::make_shared<EnemyPlantIdle>(m_parent);
	}

	return shared_from_this();
}
