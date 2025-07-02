#pragma once
#include "PlayerState.h"

class PlayerSlashDown : public PlayerState
{
public:
	PlayerSlashDown(std::weak_ptr<Player> parent);
	~PlayerSlashDown();

	std::shared_ptr<PlayerState> Update() override;

private:

	// “ü—Í‚µ‚½‚©
	bool m_isEnterAttack;
	int m_frame;
};

