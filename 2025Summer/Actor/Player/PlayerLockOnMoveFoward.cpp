#include "PlayerLockOnMoveFoward.h"

PlayerLockOnMoveFoward::PlayerLockOnMoveFoward(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
}

PlayerLockOnMoveFoward::~PlayerLockOnMoveFoward()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveFoward::Update()
{
	return shared_from_this();
}
