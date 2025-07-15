#pragma once
#include "PlayerState.h"
class PlayerLockOnMoveLeft : public PlayerState
{
public:
	PlayerLockOnMoveLeft(std::weak_ptr<Player> parent);
	~PlayerLockOnMoveLeft();

	std::shared_ptr<PlayerState> Update() override;

private:
};

