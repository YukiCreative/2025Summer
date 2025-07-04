#pragma once
#include "PlayerAttackState.h"

class PlayerSlashDown : public PlayerAttackState
{
public:
	PlayerSlashDown(std::weak_ptr<Player> parent);
	~PlayerSlashDown();

	void Init() override;

private:
};

