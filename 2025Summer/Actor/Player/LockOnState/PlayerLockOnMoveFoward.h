#pragma once
#include "PlayerLockOnMove.h"

class PlayerLockOnMoveFoward : public PlayerLockOnMove
{
public:
	PlayerLockOnMoveFoward(std::weak_ptr<Player> parent);
	~PlayerLockOnMoveFoward();

private:

	std::shared_ptr<PlayerState> OnAttack() override;
};

