#pragma once
#include "PlayerState.h"

// ƒvƒŒƒCƒ„[‚ÌUŒ‚‚Å(PlayerAttackState‚İ‚½‚¢‚È)ˆ—‚ğ‘©‚Ë‚ç‚ê‚»‚¤‚¾‚¯‚Ç¸”s‚µ‚»‚¤‚È‚Ì‚Å‚â‚ç‚È‚¢
class PlayerSlashUp : public PlayerState
{
public:
	PlayerSlashUp(std::weak_ptr<Player> parent);
	~PlayerSlashUp();

	std::shared_ptr<PlayerState> Update() override;

private:

	bool m_isEnterAttack;
	int m_frame;
};

