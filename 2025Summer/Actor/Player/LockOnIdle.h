#pragma once
#include "PlayerState.h"

class LockOnIdle : public PlayerState
{
public:
	LockOnIdle(std::weak_ptr<Player> parent);
	~LockOnIdle();

	std::shared_ptr<PlayerState> Update() override;

private:
};