#pragma once
#include "PlayerAttackState.h"

class PlayerSlashSide : public PlayerAttackState
{
public:
	PlayerSlashSide(std::weak_ptr<Player> parent);
	~PlayerSlashSide();

	void Init() override;

private:

};

