#include "PlayerDodge.h"

PlayerDodge::PlayerDodge(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
}

PlayerDodge::~PlayerDodge()
{
}

std::shared_ptr<PlayerState> PlayerDodge::Update()
{
	return shared_from_this();
}
