#pragma once
#include "PlayerAttackState.h"

// プレイヤーの攻撃で(PlayerAttackStateみたいな)処理を束ねられそうだけど失敗しそうなのでやらない
class PlayerSlashUp : public PlayerAttackState
{
public:
	PlayerSlashUp(std::weak_ptr<Player> parent);
	~PlayerSlashUp();

	void Init() override;

private:

};

