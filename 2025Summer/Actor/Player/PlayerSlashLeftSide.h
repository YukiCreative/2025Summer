#pragma once
#include "PlayerState.h"

class PlayerSlashLeftSide : public PlayerState
{
public:
	PlayerSlashLeftSide(std::weak_ptr<Player> parent);
	~PlayerSlashLeftSide();

	std::shared_ptr<PlayerState> Update() override;

private:

	bool m_isEnterAttack;
	int m_frame;
};

