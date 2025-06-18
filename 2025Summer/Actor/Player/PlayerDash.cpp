#include "Input.h"
#include "Player.h"
#include "PlayerDash.h"
#include "PlayerIdle.h"
#include "PlayerJump.h"
#include "PlayerMove.h"

namespace
{
	constexpr float kDashSpeed = 0.0015f;
}

PlayerDash::PlayerDash(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
}

PlayerDash::~PlayerDash()
{
}

std::shared_ptr<PlayerState> PlayerDash::Update()
{
	Input& input = Input::GetInstance();

	m_player.lock()->Move(kDashSpeed);
	m_player.lock()->CameraMove();

	if (input.GetLeftInputAxis().SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	if (input.IsTrigger("Jump"))
	{
		return std::make_shared<PlayerJump>(m_player);
	}

	return shared_from_this();
}
