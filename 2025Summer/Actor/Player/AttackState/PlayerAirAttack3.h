#pragma once
#include "PlayerAttackState.h"

class PlayerAirAttack3 : public PlayerAttackState
{
public:
	PlayerAirAttack3(std::weak_ptr<Player> player);
	~PlayerAirAttack3();

	void Init() override;

private:

	Vector3 m_beforeVel = {};

private:

	std::shared_ptr<PlayerState> OptionalProcess() override;
};

