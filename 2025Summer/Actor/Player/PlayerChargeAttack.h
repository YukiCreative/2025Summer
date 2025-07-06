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

	void OptionalProcess() override;
};

