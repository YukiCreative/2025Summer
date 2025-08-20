#pragma once
#include "PlayerAttackState.h"

class PlayerAirAttack3 : public PlayerAttackState
{
public:
	PlayerAirAttack3(std::weak_ptr<Player> player);
	~PlayerAirAttack3();

	void Init() override;

private:

	void OptionalProcess() override;
};

