#pragma once
#include "PlayerState.h"

// ロックオン中の状態を制御
class PlayerLockOn : public PlayerState
{
public:
	PlayerLockOn(std::weak_ptr<Player> parent);
	~PlayerLockOn();

	std::shared_ptr<PlayerState> Update() override;

private:

	std::shared_ptr<PlayerState> m_childState;

private:

	void ReleaseLockOn();
	void CameraMove();
};

