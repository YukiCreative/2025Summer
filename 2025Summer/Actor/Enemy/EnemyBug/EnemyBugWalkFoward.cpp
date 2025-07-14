#include "EnemyBugWalkFoward.h"
#include "EnemyBug.h"
#include "EnemyBugIdle.h"
#include <DxLib.h>
#include "Geometry.h"
#include "EnemyBugAttack.h"

namespace
{
	const std::string kAnimName = "Armature|Walk";
	constexpr float kWalkSpeed = 0.5f;
	constexpr float kStopWalkDistance = 200.0f;
	constexpr float kRotateThreshold = DX_PI_F * 0.1f;
}

EnemyBugWalkFoward::EnemyBugWalkFoward(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent),
	m_rotateFrame(0),
	m_rotateTimeLength(0)
{
	// アニメーション
	m_parent.lock()->ChangeAnim(kAnimName, true);
}

EnemyBugWalkFoward::~EnemyBugWalkFoward()
{
}

std::shared_ptr<EnemyBugState> EnemyBugWalkFoward::Update()
{
	// 接近
	auto parent = m_parent.lock();

	// ランダム間隔でプレイヤーのほうを向く
	// ずっと向いていると不自然なので
	if (m_rotateFrame == 0)
	{
		// 回転時間を回復
		m_rotateTimeLength = 40;
		// 次の回転回復時間を設定
		m_rotateFrame = GetRand(60) + m_rotateTimeLength;
	}
	if (m_rotateTimeLength > 0)
	{
		parent->LookAtPlayer();
	}

	--m_rotateFrame;
	--m_rotateTimeLength;

	const bool isClosePlayer = parent->EnemyToPlayer().SqrMagnitude() < kStopWalkDistance * kStopWalkDistance;
	const bool canHitAttackRotate = Geometry::Corner(parent->EnemyToPlayer().XZ(), parent->GetDir().XZ()) < kRotateThreshold;

	// 近づく方向に移動できて、今遠いなら自分の向きに前進
	if (!isClosePlayer)
	{
		if (canHitAttackRotate)
		{
			parent->AddVel(VTransformSR({0,0,kWalkSpeed}, parent->GetModelMatrix()));
		}
	}
	else
	{
		// 十分に近づいていたら攻撃タイマーを進める
		parent->CountAttackFrame();
	}

	// 一定距離に近づいた&向きもそろったなら歩くのをやめる
	if (isClosePlayer && canHitAttackRotate)
	{
		return std::make_shared<EnemyBugIdle>(m_parent);
	}

	// 攻撃遷移
	if (parent->GetAttackFrame() < 0)
	{
		return std::make_shared<EnemyBugAttack>(m_parent);
	}

	return shared_from_this();
}
