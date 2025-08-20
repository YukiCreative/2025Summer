#pragma once
#include "PlayerLockOnMove.h"

class PlayerLockOnMoveLeft : public PlayerLockOnMove
{
public:
	PlayerLockOnMoveLeft(std::weak_ptr<Player> parent);
	~PlayerLockOnMoveLeft();

private:

	std::shared_ptr<PlayerState> OnAttack() override;
};