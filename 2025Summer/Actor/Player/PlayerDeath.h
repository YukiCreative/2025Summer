#pragma once
#include "PlayerState.h"

// 倒れる、そして死ぬ！
class PlayerDeath : public PlayerState
{
public:

	PlayerDeath(std::weak_ptr<Player> parent);
	~PlayerDeath();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_frame;

};

