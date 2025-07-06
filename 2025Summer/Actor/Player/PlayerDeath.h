#pragma once
#include "PlayerState.h"

// “|‚ê‚éA‚»‚µ‚Ä€‚ÊI
class PlayerDeath : public PlayerState
{
public:

	PlayerDeath(std::weak_ptr<Player> parent);
	~PlayerDeath();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_frame;

};

