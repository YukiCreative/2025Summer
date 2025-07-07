#include "EnemyBugWalkFoward.h"
#include "EnemyBug.h"

namespace
{
	const std::string kAnimName = "Armature|Walk";
}

EnemyBugWalkFoward::EnemyBugWalkFoward(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	// �A�j���[�V����
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyBugWalkFoward::~EnemyBugWalkFoward()
{
}

std::shared_ptr<EnemyBugState> EnemyBugWalkFoward::Update()
{
	return shared_from_this();
}
