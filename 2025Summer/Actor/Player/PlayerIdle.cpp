#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerJump.h"
#include "PlayerMove.h"
#include "AnimationModel.h"

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
	m_player.lock()->CameraMove();

	// “ü—Í‚ª‚ ‚Á‚½‚ç
	if (input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
	{
		return std::make_shared<PlayerMove>(m_player);
	}
	if (input.IsTrigger("Jump"))
	{
		return std::make_shared<PlayerJump>(m_player);
	}

	return shared_from_this();
}
