#pragma once
#include "PlayerState.h"

// ”í’e
class PlayerMiddleDamage : public PlayerState
{
public:

	PlayerMiddleDamage(std::shared_ptr<Player> parent);
	~PlayerMiddleDamage();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_frame;

};

