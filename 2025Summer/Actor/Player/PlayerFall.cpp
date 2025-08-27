#include "PlayerFall.h"
#include "Player.h"
#include "PlayerLanding.h"
#include "../../General/Input.h"
#include "AttackState/PlayerAirAttack1.h"
#include "PlayerTeleportation.h"

namespace
{
	const std::string kStunAnimName = "Armature|AirIdle";
	constexpr float kMoveSpeed = 0.0015f;
}

PlayerFall::PlayerFall(std::weak_ptr<Player> player) :
	PlayerState(player),
	m_frame(0)
{
	m_player.lock()->ChangeAnim(kStunAnimName, true);
}

PlayerFall::~PlayerFall()
{
}

std::shared_ptr<PlayerState> PlayerFall::Update()
{
	// キャラクターが地面についたら遷移
	auto p = m_player.lock();
	Input& input = Input::GetInstance();

	if (p->IsGround())
	{
		return std::make_shared<PlayerLanding>(m_player);
	}
	if (input.IsTrigger("Attack"))
	{
		return std::make_shared<PlayerAirAttack1>(m_player);
	}
	if (input.IsTrigger("Tele"))
	{
		return std::make_shared<PlayerTeleportation>(m_player);
	}

	// 移動できる
	p->Move(kMoveSpeed);

	return shared_from_this();
}
