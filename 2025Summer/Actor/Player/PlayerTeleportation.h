#pragma once
#include "PlayerState.h"

// ロックオンしている対象に素早く移動する
// ロックオンしていないときは後ろに移動
class PlayerTeleportation : public PlayerState
{
public:
	PlayerTeleportation(std::weak_ptr<Player> player);
	~PlayerTeleportation();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_frame;

};

