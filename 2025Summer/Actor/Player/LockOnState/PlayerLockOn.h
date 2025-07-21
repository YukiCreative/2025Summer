#pragma once
#include "PlayerInterMediateState.h"
#include "RangeLimitedValue.h"

// ƒƒbƒNƒIƒ“’†‚Ìó‘Ô‚ğ§Œä
class PlayerLockOn : public PlayerIntermediateState
{
public:
	PlayerLockOn(std::weak_ptr<Player> parent, std::shared_ptr<PlayerState> initState);
	~PlayerLockOn();

	std::shared_ptr<PlayerIntermediateState> Update() override;

private:

	using TargetPosLerpParam = RangeLimitedValue<float, 0.0f, 1.0f>;

	TargetPosLerpParam m_targetPosLerpParam;

private:

	void CameraMove();
	void SetTargetPos();
};

