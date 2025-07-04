#include "PlayerState.h"
#include "Player.h"
#include <cmath>
#include <DxLib.h>
#include "Input.h"
#include "AnimationModel.h"
#include "ActorController.h"
#include "Geometry.h"
#include "Camera.h"

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
	auto p = m_player.lock();

	if (inputAxis.SqrMagnitude() > kMoveThreshold)
	{
		// “ü—Í‚ðƒJƒƒ‰‚É‰ñ“]
		Vector3 inputV = p->m_camera.lock()->RotateVecToCameraDirXZ({ -inputAxis.x, 0, inputAxis.y, }, Vector3::Back());

		vel = inputV.GetNormalize() * strength;
	}
	else
	{
		// ‚»‚¤‚Å‚È‚¯‚ê‚Î‹ß‚­‚Ì“G‚Ì•ûŒü
		auto nearestActor = p->m_cont.lock()->GetNearestLockOnActor(m_player.lock()->GetPos());
		auto posToNearest = nearestActor->GetPos() - p->GetPos();
		if (posToNearest.SqrMagnitude() < 100000)
		{
			vel = posToNearest.GetNormalize() * strength;
		}
		else
		{
			// ‚³‚ç‚Éˆê’è”ÍˆÍ“à‚É“G‚à‚¢‚È‚©‚Á‚½‚çŒ»Ý‚Ìƒ‚ƒfƒ‹‚ÌŒü‚«‚É‘Oi
			vel = VTransformSR({ 0,0,strength }, m_player.lock()->GetModelMatrix());
		}
	}

	auto c = Geometry::Corner(p->m_model->GetDirection(), vel.XZ());

	// •„†‚ð‚Â‚¯‚é
	if (p->m_model->GetDirection().Cross(vel.XZ()).y < 0)
	{
		c *= -1;
	}

	// ƒ‚ƒfƒ‹‚ð‰ñ“]‚³‚¹‚é
	p->m_model->RotateUpVecY(c);

	return vel;
}
