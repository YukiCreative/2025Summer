#include "EnemyEliteBite.h"
#include "EnemyElite.h"
#include "EnemyEliteIdle.h"

namespace
{
	const std::string kStartAnimName = "Armature|Found";
	const std::string kAttackAnimName = "Armature|Bite";

	constexpr int kAttackFrame = 130;
	constexpr float kRotateSpeed = 0.2f;
}

EnemyEliteBite::EnemyEliteBite(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kStartAnimName, false);
}

EnemyEliteBite::~EnemyEliteBite()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteBite::Update()
{
	auto parent = m_parent.lock();

	// 歩いたら避けれるぐらいのスピードで回転
	parent->LookAtPlayer(kRotateSpeed);

	if (parent->IsEndAnim())
	{
		if (parent->CheckAnimName(kStartAnimName))
		{
			parent->ChangeAnim(kAttackAnimName, false);
		}
		else
		{
			return std::make_shared<EnemyEliteIdle>(m_parent);
		}
	}

	if (m_frame == kAttackFrame)
	{
		parent->BiteAttack();
	}

	++m_frame;

	return shared_from_this();
}
