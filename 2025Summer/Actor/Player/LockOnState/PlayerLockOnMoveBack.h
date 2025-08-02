#pragma once
#include "../PlayerState.h"

class PlayerLockOnMoveBack : public PlayerState
{
public:
	PlayerLockOnMoveBack(std::weak_ptr<Player> parent);
	~PlayerLockOnMoveBack();

	std::shared_ptr<PlayerState> Update() override;

private:
};

