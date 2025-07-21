#pragma once
#include "PlayerAttackState.h"

class PlayerSpecialAttack : public PlayerAttackState
{
public:
	PlayerSpecialAttack(std::weak_ptr<Player> parent);
	~PlayerSpecialAttack();

	void Init() override;

private:

	void OptionalProcess() override;
};

