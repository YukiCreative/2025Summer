#pragma once
#include "PlayerState.h"

// ƒƒbƒNƒIƒ“’†‚Ìó‘Ô‚ğ§Œä
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

