#include "Input.h"
#include "Player.h"
#include "PlayerDash.h"
#include "PlayerIdle.h"
#include "PlayerJump.h"
#include "PlayerMove.h"
#include "Camera.h"
#include "AnimationModel.h"

namespace
{
	const std::string kAnimName = "Armature|Dash";
	constexpr float kDashSpeed = 0.0015f;
	constexpr float kDashFoV = 100;
	constexpr float kDefaultFoV = 80;
}

PlayerDash::PlayerDash(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// Ž‹–ìŠp‚ðL‚°‚é
	m_player.lock()->m_camera.lock()->SetTargetFoV(kDashFoV);
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerDash::~PlayerDash()
{
	m_player.lock()->m_camera.lock()->SetTargetFoV(kDefaultFoV);
}

std::shared_ptr<PlayerState> PlayerDash::Update()
{
	Input& input = Input::GetInstance();

	m_player.lock()->Move(kDashSpeed);

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
