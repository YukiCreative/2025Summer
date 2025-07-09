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
	// アニメーション
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyBugWalkFoward::~EnemyBugWalkFoward()
{
}

std::shared_ptr<EnemyBugState> EnemyBugWalkFoward::Update()
{
	// 接近
	auto parent = m_parent.lock();

	parent->LookAtPlayer();

	// 自分の向きに前進
	//parent->AddVel(VTransformSR({0,0,kWalkSpeed}, parent->GetModelMatrix()));

	// いってい

	return shared_from_this();
}
