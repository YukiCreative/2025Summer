#include "EnemyBugStun.h"
#include "EnemyBug.h"
#include <string>

namespace
{
	const std::string kDamageAnimName = "Armature|HitReact";
}

EnemyBugStun::EnemyBugStun(std::weak_ptr<EnemyBug> parent) :
	EnemyBugState(parent)
{
	// スタン中アニメーションを流したい
}

EnemyBugStun::~EnemyBugStun()
{
}

std::shared_ptr<EnemyBugState> EnemyBugStun::Update()
{
	std::shared_ptr<EnemyBug> parent = m_parent.lock();
	// ダメージを受けたらダメージアニメーションを流す
	if (parent->IsDamagedInThisFrame())
	{
		parent->ChangeAnim(kDamageAnimName, false);
	}

	// 打ち上げ攻撃を受けたら打ち上げ状態へ

	// 一定時間経過したら通常状態へ


	return shared_from_this();
}
