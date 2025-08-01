#include "PlayerDeath.h"
#include "Player.h"
#include "../../Model/AnimationModel.h"

namespace
{
	const std::string kAnimName = "Armature|Death";
}

PlayerDeath::PlayerDeath(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
	auto p = m_player.lock();

	p->m_model->ChangeAnimation(kAnimName, false);

	// 攻撃判定を消しておく
	p->DisableSwordCol();
	p->DisableSword();

	// 攻撃を食らわないようにする
	p->SetInvincibility(true);
	// ロックオンを解除して禁止する
	p->ReleaseLockOn();
	p->SetCanLockOn(false);

	// 念のためロックオンをしても状態が変わらないようにする
	m_canCrossState = true;
}

PlayerDeath::~PlayerDeath()
{
}

std::shared_ptr<PlayerState> PlayerDeath::Update()
{
	// モーションが終わるまで待つ！
	if (m_player.lock()->m_model->IsEnd())
	{
		// TODO:ゲームオーバー出す！

	}

	return shared_from_this();
}
