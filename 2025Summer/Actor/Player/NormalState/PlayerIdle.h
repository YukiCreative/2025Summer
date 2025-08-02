#pragma once
#include "../PlayerState.h"
class PlayerIdle : public PlayerState
{
public:
	PlayerIdle(std::weak_ptr<Player> parent);
	~PlayerIdle();

	std::shared_ptr<PlayerState> Update() override;
};

