#pragma once
#include "PlayerAttackState.h"
class PlayerJumpSlash : public PlayerAttackState
{
public:
	PlayerJumpSlash(std::weak_ptr<Player> parent);
	~PlayerJumpSlash();

	void Init() override;

	void OptionalProcess() override;

private:

};

