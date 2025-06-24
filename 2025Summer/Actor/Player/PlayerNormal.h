#pragma once
#include "PlayerState.h"

// ���b�N�I�����ĂȂ����
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

