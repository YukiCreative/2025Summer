#pragma once
#include "PlayerAttackState.h"

// �ːi�@�Ƃ����Ӗ��ł�
class PlayerChargeAttack : public PlayerAttackState
{
public:
	PlayerChargeAttack(std::weak_ptr<Player> parent);
	~PlayerChargeAttack();

	void Init() override;

private:

	void OptionalProcess() override;
};

