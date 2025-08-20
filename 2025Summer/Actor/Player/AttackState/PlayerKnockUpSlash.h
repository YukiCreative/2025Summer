#pragma once
#include "PlayerAttackState.h"

class PlayerKnockUpSlash : public PlayerAttackState
{
public:
	PlayerKnockUpSlash(std::weak_ptr<Player> parent);
	~PlayerKnockUpSlash();

	void Init() override;

private:
};

