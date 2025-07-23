#include "EnemyBugDeath.h"
#include "EnemyBug.h"
#include "Geometry.h"

namespace
{
	// せっかくモーションがあるんだから使おう
	const std::string kFrontDeath = "Armature|DownFront";
	const std::string kBackDeath  = "Armature|DownBack";
	const std::string kLeftDeath  = "Armature|DownLeft";
	const std::string kRightDeath = "Armature|DownRight";

	constexpr int kAnimFrameOffset = 60;
}

EnemyBugDeath::EnemyBugDeath(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent),
	m_animTotalFrame(0),
	m_frame(0)
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
	else if (cross.y > 0)// 右
	{
		m_parent.lock()->ChangeAnim(kRightDeath, false);
	}
	else
	{
		m_parent.lock()->ChangeAnim(kLeftDeath, false);
	}


	m_animTotalFrame = m_parent.lock()->GetAinmTotalTime();
	// 無敵化
	m_parent.lock()->SetInvincibility(true);
	m_parent.lock()->StartDissolve();
}

EnemyBugDeath::~EnemyBugDeath()
{
}

std::shared_ptr<EnemyBugState> EnemyBugDeath::Update()
{
	// アニメーションが終わってしばらくしたら
	if (m_frame > m_animTotalFrame + kAnimFrameOffset)
	{
		m_parent.lock()->OnDeath();
	}

	++m_frame;

	return shared_from_this();
}
