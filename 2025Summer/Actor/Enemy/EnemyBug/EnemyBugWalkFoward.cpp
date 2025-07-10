#include "EnemyBugWalkFoward.h"
#include "EnemyBug.h"
#include "EnemyBugIdle.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|Walk";
	constexpr float kWalkSpeed = 0.5f;
	constexpr float kStopWalkDistance = 200.0f;
}

EnemyBugWalkFoward::EnemyBugWalkFoward(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	// �A�j���[�V����
	m_parent.lock()->ChangeAnim(kAnimName, true);
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
	parent->AddVel(VTransformSR({0,0,kWalkSpeed}, parent->GetModelMatrix()));

	// ��苗���ɋ߂Â���������̂���߂�
	if (parent->EnemyToPlayer().SqrMagnitude() < kStopWalkDistance * kStopWalkDistance)
	{
		return std::make_shared<EnemyBugIdle>(m_parent);
	}

	return shared_from_this();
}
