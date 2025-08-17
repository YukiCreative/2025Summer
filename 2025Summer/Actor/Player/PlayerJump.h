#pragma once
#include "PlayerState.h"

class PlayerJump : public PlayerState
{
public:
	PlayerJump(std::weak_ptr<Player> player);
	~PlayerJump();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_frame;
};

