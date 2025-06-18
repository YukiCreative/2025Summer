#pragma once
#include "PlayerState.h"

class PlayerHovering : public PlayerState
{
public:
	PlayerHovering(std::weak_ptr<Player> parent);
	~PlayerHovering();

	std::shared_ptr<PlayerState> Update() override;
};

