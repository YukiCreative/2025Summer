#include "PlayerLockOnMoveLeft.h"
#include "AnimationModel.h"
#include "Player.h"
#include "PlayerLockOnIdle.h"
#include "PlayerLockOnMoveRight.h"

namespace
{
	const std::string kAnimName = "Armature|WalkLeft";
}

PlayerLockOnMoveLeft::PlayerLockOnMoveLeft(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// アニメーション
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerLockOnMoveLeft::~PlayerLockOnMoveLeft()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveLeft::Update()
{


	return shared_from_this();
}
