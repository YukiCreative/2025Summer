#include "PlayerLockOnMoveFoward.h"
#include "PlayerLockOnMoveBack.h"
#include "PlayerLockOnMoveRight.h"
#include "PlayerLockOnMoveLeft.h"
#include "PlayerLockOnIdle.h"
#include "../../../General/Input.h"
#include "../../../Model/AnimationModel.h"
#include "../Player.h"
#include <DxLib.h>
#include "../../../Camera/Camera.h"
#include "../AttackState/PlayerChargeAttack.h"
#include "../AttackState/PlayerShockWaveSlash.h"

namespace
{
	const std::string kAnimName = "Armature|WalkFoward";

	// ドライブ発生のための後ろ入力猶予
	constexpr int kBackInputRespite = 8;
}

PlayerLockOnMoveFoward::PlayerLockOnMoveFoward(std::weak_ptr<Player> parent) :
	PlayerLockOnMove(parent, kAnimName, PlayerInputDir::kFront)
{
}

PlayerLockOnMoveFoward::~PlayerLockOnMoveFoward()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveFoward::OnAttack()
{
	// 一定フレームの間に後ろに入力していたらドライブに派生

	bool isInputBack = false;
	int  searchDepth = 0;
	const int  searchMax = std::min(static_cast<int>(m_player.lock()->m_inputList.size()), kBackInputRespite);

	for (auto it = m_player.lock()->m_inputList.rbegin(); ; ++it)
	{
		if (searchDepth >= searchMax) break;
		// 指定の回数遡って合致する要素があるか調べる
		isInputBack |= (*it == PlayerInputDir::kBack);
		++searchDepth;
	}

	if (isInputBack)
	{
		// ドライブ
		return std::make_shared<PlayerShockWaveSlash>(m_player);
	}
	else
	{
		// 突進攻撃
		return std::make_shared<PlayerChargeAttack>(m_player);
	}
}