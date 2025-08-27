#include "../../../Model/AnimationModel.h"
#include "../../../Camera/Camera.h"
#include "../../../General/Input.h"
#include "../Player.h"
#include "PlayerLockOnIdle.h"
#include "PlayerLockOnMoveBack.h"
#include "PlayerLockOnMoveFoward.h"
#include "PlayerLockOnMoveLeft.h"
#include "PlayerLockOnMoveRight.h"
#include <DxLib.h>
#include "../AttackState/PlayerSlashDown.h"

namespace
{
	const std::string kStunAnimName = "Armature|WalkLeft";
}

PlayerLockOnMoveLeft::PlayerLockOnMoveLeft(std::weak_ptr<Player> parent) :
	PlayerLockOnMove(parent, kStunAnimName, PlayerInputDir::kLeft)
{
}

PlayerLockOnMoveLeft::~PlayerLockOnMoveLeft()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveLeft::OnAttack()
{
	return std::make_shared<PlayerSlashDown>(m_player);
}