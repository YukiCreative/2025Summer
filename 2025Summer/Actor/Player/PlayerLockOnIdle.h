#pragma once
#include "PlayerState.h"

class PlayerLockOnIdle : public PlayerState
{
public:
	PlayerLockOnIdle(std::weak_ptr<Player> parent);
	~PlayerLockOnIdle();

	std::shared_ptr<PlayerState> Update() override;

private:
};