#pragma once
#include "PlayerLockOnMove.h"

class PlayerLockOnMoveBack : public PlayerLockOnMove
{
public:
	PlayerLockOnMoveBack(std::weak_ptr<Player> parent);
	~PlayerLockOnMoveBack();

private:

	std::shared_ptr<PlayerState> OnAttack() override;
};

