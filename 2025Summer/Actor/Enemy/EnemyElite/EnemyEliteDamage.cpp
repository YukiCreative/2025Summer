#include "EnemyEliteDamage.h"
#include "EnemyElite.h"
#include "EnemyEliteIdle.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|KnockBack";
	const std::string kAnimName2 = "Armature|HitFront";
}

EnemyEliteDamage::EnemyEliteDamage(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	// (Š‡ŒÊ‚Ì’†‚É‚àŽO€‰‰ŽZŽq‘‚¯‚é‚ñ‚¾c)
	m_parent.lock()->ChangeAnim(GetRand(1) ? kAnimName : kAnimName2, false);
}

EnemyEliteDamage::~EnemyEliteDamage()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteDamage::Update()
{
	if (m_parent.lock()->IsEndAnim())
	{
		return std::make_shared<EnemyEliteIdle>(m_parent);
	}

	return shared_from_this();
}
