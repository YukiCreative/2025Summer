#pragma once
#include "PlayerAttackState.h"

// “Ëi@‚Æ‚¢‚¤ˆÓ–¡‚Å‚·
class PlayerChargeAttack : public PlayerAttackState
{
public:
	PlayerChargeAttack(std::weak_ptr<Player> parent);
	~PlayerChargeAttack();

	void Init() override;

private:

	std::shared_ptr<PlayerState> OptionalProcess() override;
};

