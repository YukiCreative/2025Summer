#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "AnimationModel.h"
#include "PlayerLockOnIdle.h"
#include "PlayerSlashDown.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
}

PlayerIdle::PlayerIdle(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// �������b�N�I�����ɂ��̏�ԂɂȂ��Ă��܂�����A
	// �A�j���[�V�����͐؂�ւ��Ȃ�

	if (m_player.lock()->m_lockOnActor.expired())
	{
		m_player.lock()->m_model->ChangeAnimation(kAnimName);
	}
}

PlayerIdle::~PlayerIdle()
{
}

std::shared_ptr<PlayerState> PlayerIdle::Update()
{
	// �������Ȃ�
	Input& input = Input::GetInstance();

	if (!m_player.lock()->m_lockOnActor.expired())
	{
		// ���b�N�I��
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}
	// ���͂���������
	if (input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
	{
		return std::make_shared<PlayerMove>(m_player);
	}
	if (input.IsTrigger("Attack"))
	{
		return std::make_shared<PlayerSlashDown>(m_player);
	}

	return shared_from_this();
}
