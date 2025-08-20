#pragma once
#include "PlayerLockOnMove.h"

class PlayerLockOnMoveRight : public PlayerLockOnMove
{
public:
	PlayerLockOnMoveRight(std::weak_ptr<Player> parent);
	~PlayerLockOnMoveRight();

private:

	std::shared_ptr<PlayerState> OnAttack() override;
};

