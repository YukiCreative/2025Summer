#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "AnimationModel.h"
#include "PlayerLockOnIdle.h"
#include "PlayerSlashDown.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
}

PlayerIdle::PlayerIdle(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// もしロックオン中にこの状態になってしまったら、
	// アニメーションは切り替えない

	if (m_player.lock()->m_lockOnActor.expired())
	{
		m_player.lock()->m_model->ChangeAnimation(kAnimName);
	}
}

PlayerIdle::~PlayerIdle()
{
}

std::shared_ptr<PlayerState> PlayerIdle::Update()
{
	// 何もしない
	Input& input = Input::GetInstance();

	if (!m_player.lock()->m_lockOnActor.expired())
	{
		// ロックオン
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}
	// 入力があったら
	if (input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
	{
		return std::make_shared<PlayerMove>(m_player);
	}
	if (input.IsTrigger("Attack"))
	{
		return std::make_shared<PlayerSlashDown>(m_player);
	}

	return shared_from_this();
}
