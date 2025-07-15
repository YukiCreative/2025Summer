#include "EnemyBugAttack.h"
#include "EnemyBug.h"
#include "EnemyBugIdle.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|Attack";

	const int kAttackFrame = 10;
	const Vector3 kAttackForce = {0.0f, -20.0f, 30.0f};
}

EnemyBugAttack::EnemyBugAttack(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent),
	m_frame(0)
{
	m_parent.lock()->ChangeAnim(kAnimName, false);
	// 次の攻撃フレームを決定
	m_parent.lock()->SetAttackFrame(m_parent.lock()->GetAttackInterval());
}

EnemyBugAttack::~EnemyBugAttack()
{
}

std::shared_ptr<EnemyBugState> EnemyBugAttack::Update()
{
	if (m_frame == kAttackFrame)
	{
		// 攻撃を生成
		m_parent.lock()->GenerateAttackCol();
		// 前進
		m_parent.lock()->AddVel(VTransformSR(kAttackForce, m_parent.lock()->GetModelMatrix()));
	}

	// アニメーションが終わったらIdleに
	if (m_parent.lock()->IsAnimEnd())
	{
		return std::make_shared<EnemyBugIdle>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
