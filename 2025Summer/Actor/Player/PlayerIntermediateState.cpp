#include "PlayerIntermediateState.h"
#include "PlayerMiddleDamage.h"
#include "PlayerDeath.h"

PlayerIntermediateState::PlayerIntermediateState(std::weak_ptr<Player> parent) :
	m_player(parent)
{
}

PlayerIntermediateState::~PlayerIntermediateState()
{
}

void PlayerIntermediateState::SetStateDamage()
{
	m_childState = std::make_shared<PlayerMiddleDamage>(m_player);
}

void PlayerIntermediateState::SetStateDeath()
{
	m_childState = std::make_shared<PlayerDeath>(m_player);
}

void PlayerIntermediateState::UpdateChildState()
{
	// �����Ŗ��[�̃X�e�[�g�𑖂点��
	m_childState = m_childState->Update();
}