#pragma once
#include "PlayerIntermediateState.h"

// ���b�N�I�����ĂȂ����
class PlayerNormal : public PlayerIntermediateState
{
public:
	PlayerNormal(std::weak_ptr<Player> parent);
	~PlayerNormal();

	std::shared_ptr<PlayerIntermediateState> Update() override;

private:

private:

	void LockOn();
	void MoveCameraTarget();
};

