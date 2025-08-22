#pragma once
#include "PlayerAttackState.h"

class PlayerAirAttack2 : public PlayerAttackState
{
public:
	PlayerAirAttack2(std::weak_ptr<Player> player);
	~PlayerAirAttack2();

	void Init() override;

private:

	Vector3 m_beforeVel = {};

private:

	void OptionalProcess() override;
};

