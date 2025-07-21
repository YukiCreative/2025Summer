#pragma once
#include "PlayerAttackState.h"

class PlayerSpecialAttack : public PlayerState
{
public:
	PlayerSpecialAttack(std::weak_ptr<Player> parent);
	~PlayerSpecialAttack();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_frame;
};

