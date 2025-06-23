#include "PlayerLockOnMoveBack.h"

PlayerLockOnMoveBack::PlayerLockOnMoveBack(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
}

PlayerLockOnMoveBack::~PlayerLockOnMoveBack()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveBack::Update()
{
	return shared_from_this();
}
