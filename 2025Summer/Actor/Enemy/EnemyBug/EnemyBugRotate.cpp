#include "EnemyBugRotate.h"
#include "EnemyBug.h"

namespace
{
	const std::string kAnimName = "Armature|Walk";
}

EnemyBugRotate::EnemyBugRotate(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	// •à‚«ƒAƒjƒ[ƒVƒ‡ƒ“‚Å‰ñ“]‚µ‚½•—‚ÉŒ©‚¹‚é
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyBugRotate::~EnemyBugRotate()
{
}

std::shared_ptr<EnemyBugState> EnemyBugRotate::Update()
{
	// ‰ñ“]
	m_parent.lock()->LookAtPlayer();

	// ‚à‚µ

	return shared_from_this();
}

