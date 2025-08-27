#include "../../../General/Input.h"
#include "../Player.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "../../../Model/AnimationModel.h"
#include "../LockOnState/PlayerLockOnIdle.h"
#include "../AttackState/PlayerSlashDown.h"
#include "../PlayerDodge.h"
#include "../PlayerJump.h"
#include "../PlayerFall.h"
#include "../PlayerTeleportation.h"

namespace
{
	const std::string kStunAnimName = "Armature|Idle";
}

PlayerIdle::PlayerIdle(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// もしロックオン中にこの状態になってしまったら、
	// アニメーションは切り替えない

	if (m_player.lock()->m_lockOnActor.expired())
	{
		m_player.lock()->m_model->ChangeAnimation(kStunAnimName);
	}
}

PlayerIdle::~PlayerIdle()
{
}

std::shared_ptr<PlayerState> PlayerIdle::Update()
{
	// 何もしない
	Input& input = Input::GetInstance();

	if (!m_player.lock()->IsGround())
	{
		return std::make_shared<PlayerFall>(m_player);
	}
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
	if (input.IsTrigger("Dodge"))
	{
		return std::make_shared<PlayerDodge>(m_player);
	}
	if (input.IsTrigger("Jump"))
	{
		return std::make_shared<PlayerJump>(m_player);
	}
	if (input.IsTrigger("Tele"))
	{
		return std::make_shared<PlayerTeleportation>(m_player);
	}

	return shared_from_this();
}
