#pragma once
#include "PlayerAttackState.h"

class PlayerSlashLeftSide : public PlayerAttackState
{
public:
	PlayerSlashLeftSide(std::weak_ptr<Player> parent);
	~PlayerSlashLeftSide();

	void Init() override;

private:
};

