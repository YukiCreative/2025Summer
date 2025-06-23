#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerJump.h"
#include "PlayerMove.h"
#include "AnimationModel.h"
#include "LockOnIdle.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
}

PlayerIdle::PlayerIdle(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerIdle::~PlayerIdle()
{
}

std::shared_ptr<PlayerState> PlayerIdle::Update()
{
	// ‰½‚à‚µ‚È‚¢
	Input& input = Input::GetInstance();

	MoveCameraTarget();

	// “ü—Í‚ª‚ ‚Á‚½‚ç
	if (input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
	{
		return std::make_shared<PlayerMove>(m_player);
	}
	if (input.IsTrigger("Jump"))
	{
		return std::make_shared<PlayerJump>(m_player);
	}
	if (!m_player.lock()->m_lockOnActor.expired())
	{
		return std::make_shared<LockOnIdle>(m_player);
	}

	return shared_from_this();
}
