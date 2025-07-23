#pragma once
#include "PlayerAttackState.h"

class PlayerFinishAttack : public PlayerAttackState
{
public:
	PlayerFinishAttack(std::weak_ptr<Player> parent);
	~PlayerFinishAttack();

	void Init() override;

private:
};

