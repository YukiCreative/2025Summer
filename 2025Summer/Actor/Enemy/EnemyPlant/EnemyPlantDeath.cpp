#include "EnemyPlantDeath.h"
#include "EnemyPlant.h"
#include "Geometry.h"

namespace
{
	const std::string kFrontDeath = "Armature|DownFront";
	const std::string kBackDeath  = "Armature|DownBack";
	const std::string kRightDeath = "Armature|DownRight";
	const std::string kLeftDeath  = "Armature|DownLeft";
}

EnemyPlantDeath::EnemyPlantDeath(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// アニメーション
	// プレイヤーとの向きによって倒れる向きを変えよう
	auto eToPN = m_parent.lock()->EnemyToPlayer().GetNormalize().XZ();
	auto dir = -m_parent.lock()->GetDir().XZ(); // なぜか逆になるので向きを逆にした
	auto eToPRad = Geometry::Corner(eToPN, dir);
	auto cross = dir.Cross(eToPN);

	// 前
	if (eToPRad <= DX_PI_F * 0.25f)
	{
		m_parent.lock()->ChangeAnim(kFrontDeath, false);
	}
	else if (eToPRad >= DX_PI_F * 0.75f) // 後ろ
	{
		m_parent.lock()->ChangeAnim(kBackDeath, false);
	}
	else if (cross.y < 0)// 右
	{
		m_parent.lock()->ChangeAnim(kRightDeath, false);
	}
	else
	{
		m_parent.lock()->ChangeAnim(kLeftDeath, false);
	}

	// 無敵化
	m_parent.lock()->SetInvincibility(true);
}

EnemyPlantDeath::~EnemyPlantDeath()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantDeath::Update()
{
	// アニメーションが終わるまで待機
	if (m_parent.lock()->IsEndAnim())
	{
		m_parent.lock()->OnDeath();
	}

	return shared_from_this();
}
