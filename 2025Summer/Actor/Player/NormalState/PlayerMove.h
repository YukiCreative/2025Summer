#pragma once
#include "PlayerState.h"

class PlayerMove : public PlayerState
{
public:
	PlayerMove(std::weak_ptr<Player> parent);
	~PlayerMove();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_moveFrame;
};

