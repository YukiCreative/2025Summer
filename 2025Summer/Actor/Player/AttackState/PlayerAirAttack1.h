#pragma once
#include "PlayerAttackState.h"

class PlayerAirAttack1 : public PlayerAttackState
{
public:
	PlayerAirAttack1(std::weak_ptr<Player> player);
	~PlayerAirAttack1();

	void Init() override;

private:

	void OptionalProcess() override;
};

