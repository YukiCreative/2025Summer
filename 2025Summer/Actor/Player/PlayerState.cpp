#include "PlayerState.h"
#include "Player.h"
#include <cmath>

PlayerState::PlayerState(std::weak_ptr<Player> parent)
{
	m_player = parent;
}

PlayerState::~PlayerState()
{
}

void PlayerState::MoveCameraTarget()
{
	// �ӂ��͂���
	// �K�v�ɉ�����override����
	m_player.lock()->m_targetPos = m_player.lock()->GetPos() + kCameraTargetOffset;
}

void PlayerState::MoveCameraTargetIgnoreY(const float includeRatio)
{
	// Y�Ǐ]���Ȃ�

	const Vector3 temp = m_player.lock()->m_targetPos;

	m_player.lock()->m_targetPos = m_player.lock()->GetPos() + kCameraTargetOffset;
	m_player.lock()->m_targetPos.y = std::lerp(temp.y, m_player.lock()->m_targetPos.y, includeRatio);
}
