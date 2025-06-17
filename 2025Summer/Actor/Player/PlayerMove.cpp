#include "PlayerMove.h"
#include "Player.h"
#include "Input.h"
#include "PlayerIdle.h"

namespace
{
	constexpr float kMoveThreshold = 0.001f;
}

PlayerMove::PlayerMove(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{

}

PlayerMove::~PlayerMove()
{
}

std::shared_ptr<PlayerState> PlayerMove::Update()
{
	Input& input = Input::GetInstance();

	// �ړ�
	m_player.lock()->Move();
	m_player.lock()->CameraMove();

	// ���͂��؂ꂽ��ҋ@��Ԃ�
	if (input.GetLeftInputAxis().SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerIdle>();
	}

	return shared_from_this();
}
