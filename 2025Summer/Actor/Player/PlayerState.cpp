#include "PlayerState.h"
#include "Player.h"
#include <cmath>
#include <DxLib.h>
#include "Input.h"
#include "AnimationModel.h"

PlayerState::PlayerState(std::weak_ptr<Player> parent) :
	m_canCrossState(false)
{
	m_player = parent;
}

PlayerState::~PlayerState()
{
}