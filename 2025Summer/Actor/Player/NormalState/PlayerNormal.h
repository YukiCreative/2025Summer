#pragma once
#include "../PlayerIntermediateState.h"

// ロックオンしてない状態
class PlayerNormal : public PlayerIntermediateState
{
public:
	PlayerNormal(std::weak_ptr<Player> parent, std::shared_ptr<PlayerState> initState);
	~PlayerNormal();

	std::shared_ptr<PlayerIntermediateState> Update() override;

private:

private:

	void LockOn();
	void MoveCameraTarget();
};

