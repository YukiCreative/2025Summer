#pragma once
#include "PlayerState.h"

class PlayerSlashSide : public PlayerState
{
public:
	PlayerSlashSide(std::weak_ptr<Player> parent);
	~PlayerSlashSide();

	std::shared_ptr<PlayerState> Update() override;

private:

	bool m_isEnterAttack;
	int m_frame;
};

