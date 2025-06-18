#pragma once
#include "PlayerState.h"
class PlayerDash : public PlayerState
{
public:
	PlayerDash(std::weak_ptr<Player> parent);
	~PlayerDash();

	std::shared_ptr<PlayerState> Update() override;
};

