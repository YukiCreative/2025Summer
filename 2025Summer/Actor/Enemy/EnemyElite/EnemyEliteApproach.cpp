#include "EnemyEliteApproach.h"
#include "EnemyElite.h"
#include "EnemyEliteIdle.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|Run";
	constexpr float kStopDistance = 500.0f;
	constexpr float kMoveSpeed = 1.0f;
	constexpr float kLookSpeed = 0.3f;
}

EnemyEliteApproach::EnemyEliteApproach(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyEliteApproach::~EnemyEliteApproach()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteApproach::Update()
{
	auto parent = m_parent.lock();

	parent->LookAtPlayer(kLookSpeed);

	// Ú‹ß
	parent->AddVel(VTransformSR({ 0, 0, kMoveSpeed }, parent->GetModelMatrix()));

	if (parent->EnemyToPlayer().SqrMagnitude() < kStopDistance * kStopDistance)
	{
		return std::make_shared<EnemyEliteIdle>(m_parent);
	}

	return shared_from_this();
}
