#pragma once
#include "PlayerState.h"

class PlayerLockOnMoveRight : public PlayerState
{
public:
	PlayerLockOnMoveRight(std::weak_ptr<Player> parent);
	~PlayerLockOnMoveRight();

	std::shared_ptr<PlayerState> Update() override;

private:
};

