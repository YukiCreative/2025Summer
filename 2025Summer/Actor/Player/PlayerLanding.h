#pragma once
#include "PlayerState.h"
class PlayerLanding : public PlayerState
{
public:
	PlayerLanding(std::weak_ptr<Player> parent);
	~PlayerLanding();

	std::shared_ptr<PlayerState> Update() override;

private:
	int m_landFrame;
};

