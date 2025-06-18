#include "Input.h"
#include "Player.h"
#include "PlayerDash.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"

namespace
{
	constexpr float kRunSpeed = 0.001f;
	constexpr int   kDashFrame = 120;
}

PlayerMove::PlayerMove(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_moveFrame(0)
{

}

PlayerMove::~PlayerMove()
{
}

std::shared_ptr<PlayerState> PlayerMove::Update()
{
	Input& input = Input::GetInstance();

	// �ړ�
	m_player.lock()->Move(kRunSpeed);
	m_player.lock()->CameraMove();

	// ���͂��؂ꂽ��ҋ@��Ԃ�
	if (input.GetLeftInputAxis().SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	// ���������Ĉ�莞�Ԍo�߂�����_�b�V��
	if (m_moveFrame > kDashFrame)
	{
		return std::make_shared<PlayerDash>(m_player);
	}

	++m_moveFrame;

	return shared_from_this();
}
