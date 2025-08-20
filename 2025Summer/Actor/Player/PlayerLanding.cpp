#include "PlayerLanding.h"
#include "Player.h"
#include "NormalState/PlayerIdle.h"
#include "NormalState/PlayerMove.h"
#include "../../General/Input.h"
#include "AttackState/PlayerSlashDown.h"

namespace
{
	const std::string kAnimName = "Armature|Landing";
	constexpr int kEnableMoveFrame = 10;
	constexpr float kMoveSpeed = 0.001f;
}

PlayerLanding::PlayerLanding(std::weak_ptr<Player> player) :
	PlayerState(player),
	m_frame(0)
{
	m_player.lock()->ChangeAnim(kAnimName, false);
}

PlayerLanding::~PlayerLanding()
{
}

std::shared_ptr<PlayerState> PlayerLanding::Update()
{
	auto p = m_player.lock();
	Input& input = Input::GetInstance();

	if (m_frame > kEnableMoveFrame && input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
	{
		return std::make_shared<PlayerMove>(m_player);
	}

	if (p->IsAnimEnd())
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	if (input.IsTrigger("Attack"))
	{
		return std::make_shared<PlayerSlashDown>(m_player);
	}

	p->Move(kMoveSpeed);

	++m_frame;

	return shared_from_this();
}
