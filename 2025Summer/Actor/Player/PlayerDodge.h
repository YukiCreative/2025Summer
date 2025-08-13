#pragma once
#include "PlayerState.h"

class PlayerDodge : public PlayerState
{
public:

	PlayerDodge(std::weak_ptr<Player> parent);
	~PlayerDodge();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_frame;
};

