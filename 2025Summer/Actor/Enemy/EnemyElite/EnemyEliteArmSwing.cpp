#include "EnemyEliteArmSwing.h"
#include "EnemyElite.h"
#include "EnemyEliteIdle.h"

namespace
{
	const std::string kAnimName = "Armature|ArmSwing";
	constexpr int kAttackFrame = 50;
	constexpr float kRotateSpeed = 0.2f;
}

EnemyEliteArmSwing::EnemyEliteArmSwing(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kAnimName, false);
}

EnemyEliteArmSwing::~EnemyEliteArmSwing()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteArmSwing::Update()
{
	auto parent = m_parent.lock();

	// 歩いたら避けれるぐらいのスピードで回転
	parent->LookAtPlayer(kRotateSpeed);

	if (m_frame == kAttackFrame)
	{
		parent->ArmSwingAttack();
	}

	if (parent->IsEndAnim())
	{
		return std::make_shared<EnemyEliteIdle>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
