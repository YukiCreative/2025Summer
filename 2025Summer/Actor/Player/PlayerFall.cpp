#include "PlayerFall.h"
#include "Player.h"
#include "NormalState/PlayerIdle.h"

namespace
{
	const std::string kAnimName = "Armature|AirIdle";
}

PlayerFall::PlayerFall(std::weak_ptr<Player> player) :
	PlayerState(player),
	m_frame(0)
{
	m_player.lock()->ChangeAnim(kAnimName, true);
}

PlayerFall::~PlayerFall()
{
}

std::shared_ptr<PlayerState> PlayerFall::Update()
{
	// キャラクターが地面についたら遷移
	auto p = m_player.lock();

	if (p->IsGround())
	{
		return std::make_shared<PlayerIdle>(m_player);
	}

	return shared_from_this();
}
