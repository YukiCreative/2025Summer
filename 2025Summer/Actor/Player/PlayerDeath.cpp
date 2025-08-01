#include "PlayerDeath.h"
#include "Player.h"
#include "../../Model/AnimationModel.h"

namespace
{
	const std::string kAnimName = "Armature|Death";
}

PlayerDeath::PlayerDeath(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
	auto p = m_player.lock();

	p->m_model->ChangeAnimation(kAnimName, false);

	// �U������������Ă���
	p->DisableSwordCol();
	p->DisableSword();

	// �U����H���Ȃ��悤�ɂ���
	p->SetInvincibility(true);
	// ���b�N�I�����������ċ֎~����
	p->ReleaseLockOn();
	p->SetCanLockOn(false);

	// �O�̂��߃��b�N�I�������Ă���Ԃ��ς��Ȃ��悤�ɂ���
	m_canCrossState = true;
}

PlayerDeath::~PlayerDeath()
{
}

std::shared_ptr<PlayerState> PlayerDeath::Update()
{
	// ���[�V�������I���܂ő҂I
	if (m_player.lock()->m_model->IsEnd())
	{
		// TODO:�Q�[���I�[�o�[�o���I

	}

	return shared_from_this();
}
