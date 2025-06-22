#include "PlayerState.h"
#include "Player.h"
#include <cmath>
#include <DxLib.h>

PlayerState::PlayerState(std::weak_ptr<Player> parent)
{
	m_player = parent;
}

PlayerState::~PlayerState()
{
}

void PlayerState::MoveCameraTarget()
{
	// ‚Ó‚Â‚¤‚Í‚±‚ê
	// •K—v‚É‰ž‚¶‚Äoverride‚µ‚Ä

	if (m_player.lock()->m_lockOnActor.expired())
	{
		m_player.lock()->m_targetPos = m_player.lock()->GetPos() + kCameraTargetOffset;
	}
	else
	{
		// ‚È‚°‚¦
		m_player.lock()->m_targetPos = (m_player.lock()->GetPos() + kCameraTargetOffset + m_player.lock()->m_lockOnActor.lock()->GetPos()) * 0.5f;
	}
}

void PlayerState::MoveCameraTargetIgnoreY(const float includeRatio)
{
	// Y’Ç]‚µ‚È‚¢

	const Vector3 beforeTargetPos = m_player.lock()->m_targetPos;

	if (m_player.lock()->m_lockOnActor.expired())
	{
		m_player.lock()->m_targetPos = m_player.lock()->GetPos() + kCameraTargetOffset;
		m_player.lock()->m_targetPos.y = std::lerp(beforeTargetPos.y, m_player.lock()->m_targetPos.y, includeRatio);
	}
	else
	{
		const Vector3 lockOnTarget = (m_player.lock()->GetPos() + kCameraTargetOffset + m_player.lock()->m_lockOnActor.lock()->GetPos()) * 0.5f;

		m_player.lock()->m_targetPos = lockOnTarget;
		m_player.lock()->m_targetPos.y = std::lerp(beforeTargetPos.y, m_player.lock()->m_targetPos.y, includeRatio);
	}
}
