#pragma once
#include "PlayerState.h"

class PlayerJump : public PlayerState
{
public:
	PlayerJump(std::weak_ptr<Player> player);
	~PlayerJump();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_frame;
	Vector3 m_startPos;
	Vector3 m_targetPos;
	float m_time = 0;

private:

	bool CheckLockOnJump() const;
};

