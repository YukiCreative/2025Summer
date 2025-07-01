#include "Input.h"
#include "Player.h"
#include "PlayerDash.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "PlayerJump.h"
#include "AnimationModel.h"

namespace
{
	constexpr float kRunSpeed = 0.0018f;
	constexpr int   kDashFrame = 120;

	const std::string kMoveAnimName = "Armature|RunDash";
}

PlayerMove::PlayerMove(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_moveFrame(0)
{
	// �A�j���[�V������J��
	m_player.lock()->m_model->ChangeAnimation(kMoveAnimName);
}

PlayerMove::~PlayerMove()
{
}

std::shared_ptr<PlayerState> PlayerMove::Update()
{
	Input& input = Input::GetInstance();

	// �ړ�
	m_player.lock()->Move(kRunSpeed);

	MoveCameraTarget();

	// ���͂��؂ꂽ��ҋ@��Ԃ�
	if (input.GetLeftInputAxis().SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	// ���������Ĉ�莞�Ԍo�߂�����_�b�V��
	//if (m_moveFrame > kDashFrame)
	//{
	//	return std::make_shared<PlayerDash>(m_player);
	//}
	//if (input.IsTrigger("Jump"))
	//{
	//	return std::make_shared<PlayerJump>(m_player);
	//}

	++m_moveFrame;

	return shared_from_this();
}
