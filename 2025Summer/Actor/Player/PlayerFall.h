#pragma once
#include "PlayerState.h"

class PlayerFall : public PlayerState
{
public:
	PlayerFall(std::weak_ptr<Player> player);
	~PlayerFall();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_frame;
};

