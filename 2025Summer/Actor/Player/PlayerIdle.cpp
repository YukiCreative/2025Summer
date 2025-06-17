#include "PlayerIdle.h"


PlayerIdle::PlayerIdle(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{

}

PlayerIdle::~PlayerIdle()
{
}

std::shared_ptr<PlayerState> PlayerIdle::Update()
{
	return shared_from_this();
}
