#include "PlayerState.h"
#include "Player.h"
#include <cmath>
#include <DxLib.h>
#include "Input.h"
#include "AnimationModel.h"
#include "ActorController.h"

PlayerState::PlayerState(std::weak_ptr<Player> parent)
{
	m_player = parent;
}

PlayerState::~PlayerState()
{
}

void PlayerState::MoveCameraTarget()
{
	m_player.lock()->m_targetPos = m_player.lock()->GetPos() + kCameraTargetOffset;
}

void PlayerState::MoveCameraTargetIgnoreY(const float includeRatio)
{
	// Y’Ç]‚µ‚È‚¢

	const Vector3 beforeTargetPos = m_player.lock()->m_targetPos;

	m_player.lock()->m_targetPos = m_player.lock()->GetPos() + kCameraTargetOffset;
	m_player.lock()->m_targetPos.y = std::lerp(beforeTargetPos.y, m_player.lock()->m_targetPos.y, includeRatio);
}

Vector3 PlayerState::TrackingVec(const float strength)
{
	Vector3 vel;
	// “ü—Í‚ª‚ ‚Á‚½‚çA‚»‚Ì•ûŒü‚É“®‚­
	const auto& inputAxis = Input::GetInstance().GetLeftInputAxis();

	if (inputAxis.SqrMagnitude() > kMoveThreshold)
	{
		vel = VTransformSR({0,0,strength}, MGetRotVec2(m_player.lock()->m_model->GetDirection(), {inputAxis.x, 0, inputAxis.y,}));
	}
	// ‚»‚¤‚Å‚È‚¯‚ê‚Î‹ß‚­‚Ì“G‚Ì•ûŒü
	else
	{
	}
	else
	{
		// ‚³‚ç‚Éˆê’è”ÍˆÍ“à‚É“G‚à‚¢‚È‚©‚Á‚½‚çŒ»Ý‚Ìƒ‚ƒfƒ‹‚ÌŒü‚«‚É‘Oi
		vel = VTransformSR({0,0,strength}, m_player.lock()->GetModelMatrix());
	}

	// ‘¬“x‚Ì•ûŒü‚Éƒ‚ƒfƒ‹‚ð‰ñ“]‚³‚¹‚é


	return vel;
}
