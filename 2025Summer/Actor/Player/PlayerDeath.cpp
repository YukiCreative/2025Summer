#include "PlayerDeath.h"
#include "Player.h"
#include "AnimationModel.h"

namespace
{
	const std::string kAnimName = "Armature|Death";
}

PlayerDeath::PlayerDeath(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);

	// 攻撃判定を消しておく
	m_player.lock()->DisableSwordCol();
	m_player.lock()->DisableSword();
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
