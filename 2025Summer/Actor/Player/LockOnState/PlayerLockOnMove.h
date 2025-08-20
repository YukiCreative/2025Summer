#pragma once
#include "../PlayerState.h"
#include "PlayerInputDir.h"
#include <string>

class Player;

class PlayerLockOnMove : public PlayerState
{
public:
	PlayerLockOnMove(std::weak_ptr<Player> parent, const std::string& animName, const PlayerInputDir dir);
	~PlayerLockOnMove();

	std::shared_ptr<PlayerState> Update() override final;

private:

	PlayerInputDir m_dir;

private:

	// UŒ‚ƒ{ƒ^ƒ“‰Ÿ‰º‚É‚Ç‚ñ‚ÈUŒ‚‚ğ•Ô‚·‚©‚ğİ’è‚³‚¹‚é
	virtual std::shared_ptr<PlayerState> OnAttack() abstract;
};

