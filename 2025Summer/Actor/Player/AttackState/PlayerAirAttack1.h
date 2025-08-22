#pragma once
#include "PlayerAttackState.h"

class PlayerAirAttack1 : public PlayerAttackState
{
public:
	PlayerAirAttack1(std::weak_ptr<Player> player);
	~PlayerAirAttack1();

	void Init() override;

private:

	Vector3 m_beforeVel = {};

private:

	std::shared_ptr<PlayerState> OptionalProcess() override;
};

