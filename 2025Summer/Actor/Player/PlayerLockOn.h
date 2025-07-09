#pragma once
#include "PlayerState.h"
#include "RangeLimitedValue.h"

// ƒƒbƒNƒIƒ“’†‚Ìó‘Ô‚ğ§Œä
class PlayerLockOn : public PlayerState
{
public:
	PlayerLockOn(std::weak_ptr<Player> parent);
	~PlayerLockOn();

	std::shared_ptr<PlayerState> Update() override;

private:

	std::shared_ptr<PlayerState> m_childState;

	using TargetPosLerpParam = RangeLimitedValue<float, 0.0f, 1.0f>;

	TargetPosLerpParam m_targetPosLerpParam;

private:

	void ReleaseLockOn();
	void CameraMove();
	void SetTargetPos();
};

