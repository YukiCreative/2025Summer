#include "PlayerDeath.h"
#include "Player.h"
#include "AnimationModel.h"

namespace
{
	const std::string kAnimName = "Armature|Death";
}

PlayerDeath::PlayerDeath(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);

	// �U������������Ă���
	m_player.lock()->DisableSwordCol();
	m_player.lock()->DisableSword();
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
