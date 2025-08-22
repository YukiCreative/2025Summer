#pragma once
#include "PlayerAttackState.h"

class PlayerShockWaveSlash : public PlayerAttackState
{
public:
	PlayerShockWaveSlash(std::weak_ptr<Player> parent);
	~PlayerShockWaveSlash();

	void Init() override;

private:

	std::shared_ptr<PlayerState> OptionalProcess() override;
};

