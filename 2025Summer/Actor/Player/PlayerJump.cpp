#include "PlayerJump.h"
#include "Player.h"
#include "Input.h"
#include "Rigid.h"
#include "PlayerLanding.h"

namespace
{
	const Vector3 kJumpForce = {0, 50, 0};
	constexpr float kJumpTopVel = 0.05f;
}

PlayerJump::PlayerJump(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// ��Ԃ̊J�n����
	auto& rigid = m_player.lock()->GetRigid();

	rigid.ChangeStateAir();
	rigid.AddVel(kJumpForce);
}

PlayerJump::~PlayerJump()
{
}

std::shared_ptr<PlayerState> PlayerJump::Update()
{
	m_player.lock()->CameraMove();

	// �n�ʂɂ����璅�n���[�V������
	// �����蔻�肪�ł���܂ł͉�
	if (m_player.lock()->GetPos().y < 0)
	{
		return std::make_shared<PlayerLanding>(m_player);
	}
	// �W�����v�ō��_�߂��ɂȂ�����A������Ƒ؋󂵂���
	if (m_player.lock()->GetRigid().GetVel().y < kJumpTopVel)
	{
		return std::make_shared<>
	}

	return shared_from_this();
}
