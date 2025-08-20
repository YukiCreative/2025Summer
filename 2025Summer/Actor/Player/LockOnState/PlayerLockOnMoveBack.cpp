#include "../../../Model/AnimationModel.h"
#include "../../../Camera/Camera.h"
#include "../../../General/Input.h"
#include "../Player.h"
#include "PlayerLockOnIdle.h"
#include "PlayerLockOnMoveBack.h"
#include "PlayerLockOnMoveFoward.h"
#include "PlayerLockOnMoveLeft.h"
#include "PlayerLockOnMoveRight.h"
#include "../AttackState/PlayerKnockUpSlash.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|WalkBack";
}

PlayerLockOnMoveBack::PlayerLockOnMoveBack(std::weak_ptr<Player> parent) :
	PlayerLockOnMove(parent, kAnimName, PlayerInputDir::kBack)
{
}

PlayerLockOnMoveBack::~PlayerLockOnMoveBack()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveBack::OnAttack()
{
	return std::make_shared<PlayerKnockUpSlash>(m_player);
}
