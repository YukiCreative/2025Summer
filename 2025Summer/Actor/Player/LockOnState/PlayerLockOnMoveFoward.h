#pragma once
#include "PlayerState.h"

class PlayerLockOnMoveFoward : public PlayerState
{
public:
	PlayerLockOnMoveFoward(std::weak_ptr<Player> parent);
	~PlayerLockOnMoveFoward();

	std::shared_ptr<PlayerState> Update() override;

private:
};

