#include "PlayerMiddleDamage.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "AnimationModel.h"

namespace
{
	const std::string kAnimName = "Armature|MiddleDamage";
}

PlayerMiddleDamage::PlayerMiddleDamage(std::shared_ptr<Player> parent) :
	PlayerState(parent)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);
}

PlayerMiddleDamage::~PlayerMiddleDamage()
{
}

std::shared_ptr<PlayerState> PlayerMiddleDamage::Update()
{
	auto p = m_player.lock();

	// �A�j���[�V����������or�r���œ��͂���������J��
	if (p->m_model->IsEnd())
	{
		return std::make_shared<PlayerIdle>(m_player);
	}

	if ()
	{

	}

	return shared_from_this();
}
