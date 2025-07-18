#pragma once
#include "PlayerAttackState.h"

// 突進　という意味です
class PlayerChargeAttack : public PlayerAttackState
{
public:
	PlayerChargeAttack(std::weak_ptr<Player> parent);
	~PlayerChargeAttack();

	void Init() override;

private:

	void OptionalProcess() override;
};

