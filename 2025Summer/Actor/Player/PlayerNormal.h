#pragma once
#include "PlayerState.h"

// ロックオンしてない状態
class PlayerNormal : public PlayerState
{
public:
	PlayerNormal(std::weak_ptr<Player> parent);
	~PlayerNormal();

	std::shared_ptr<PlayerState> Update() override;

private:

	std::shared_ptr<PlayerState> m_childState;

private:

	void LockOn();
};

