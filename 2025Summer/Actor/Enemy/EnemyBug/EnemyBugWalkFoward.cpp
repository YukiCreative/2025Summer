#include "EnemyBugWalkFoward.h"
#include "EnemyBug.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|Walk";
	constexpr float kWalkSpeed = 0.5f;
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
	// �ڋ�
	auto parent = m_parent.lock();

	parent->LookAtPlayer();

	// �����̌����ɑO�i
	//parent->AddVel(VTransformSR({0,0,kWalkSpeed}, parent->GetModelMatrix()));

	// �����Ă�

	return shared_from_this();
}
