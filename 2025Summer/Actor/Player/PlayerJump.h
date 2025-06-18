#pragma once
#include "PlayerState.h"
class PlayerJump : public PlayerState
{
public:
	PlayerJump(std::weak_ptr<Player> parent);
	~PlayerJump();

	std::shared_ptr<PlayerState> Update() override;
};

