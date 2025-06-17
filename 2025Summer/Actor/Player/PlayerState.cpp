#include "PlayerState.h"

PlayerState::PlayerState(std::weak_ptr<Player> parent)
{
	m_player = parent;
}

PlayerState::~PlayerState()
{
}
