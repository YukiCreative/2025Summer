#include "EnemyBugDamage.h"
#include "EnemyBug.h"
#include "EnemyBugIdle.h"

namespace
{
	const std::string kanimName = "Armature|HitReact";
}

EnemyBugDamage::EnemyBugDamage(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	m_parent.lock()->ChangeAnim(kanimName, false);
}

EnemyBugDamage::~EnemyBugDamage()
{
}

std::shared_ptr<EnemyBugState> EnemyBugDamage::Update()
{
	auto parent = m_parent.lock();

	// �A�j���[�V�������I���܂őҋ@
	if (parent->IsEndAnim())
	{
		return std::make_shared<EnemyBugIdle>(m_parent);
	}

	return shared_from_this();
}
