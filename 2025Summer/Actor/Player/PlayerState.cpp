#include "PlayerState.h"
#include "Player.h"
#include <cmath>
#include <DxLib.h>
#include "Input.h"
#include "AnimationModel.h"

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
