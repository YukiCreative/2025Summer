#include "EnemyBugIdle.h"
#include "EnemyBug.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
	//constexpr float kApproachDistance = 
}

EnemyBugIdle::EnemyBugIdle(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	// アニメーション再生
	m_parent.lock()->ChangeAnim(kAnimName);
}

EnemyBugIdle::~EnemyBugIdle()
{
}

std::shared_ptr<EnemyBugState> EnemyBugIdle::Update()
{
	// プレイヤーを見る
	// 接近はしない 様子見みたいな
	auto parent = m_parent.lock();

	parent->LookAtPlayer();

	// 一定以上離れていたら接近
	//if (parent->EnemyToPlayer().SqrMagnitude() > k)

	return shared_from_this();
}
