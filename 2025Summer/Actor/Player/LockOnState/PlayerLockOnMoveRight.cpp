#include "../../../Model/AnimationModel.h"
#include "../../../Camera/Camera.h"
#include "../../../General/Input.h"
#include "../Player.h"
#include "PlayerLockOnIdle.h"
#include "PlayerLockOnMoveBack.h"
#include "PlayerLockOnMoveFoward.h"
#include "PlayerLockOnMoveLeft.h"
#include "PlayerLockOnMoveRight.h"
#include "../AttackState/PlayerSlashDown.h"

namespace
{
	const std::string kStunAnimName = "Armature|WalkRight";
}

PlayerLockOnMoveRight::PlayerLockOnMoveRight(std::weak_ptr<Player> parent) :
	PlayerLockOnMove(parent, kStunAnimName, PlayerInputDir::kRight)
{
}

PlayerLockOnMoveRight::~PlayerLockOnMoveRight()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveRight::OnAttack()
{
	return std::make_shared<PlayerSlashDown>(m_player);
}