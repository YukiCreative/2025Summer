#pragma once
#include "PlayerAttackState.h"

// ƒvƒŒƒCƒ„[‚ÌUŒ‚‚Å(PlayerAttackState‚İ‚½‚¢‚È)ˆ—‚ğ‘©‚Ë‚ç‚ê‚»‚¤‚¾‚¯‚Ç¸”s‚µ‚»‚¤‚È‚Ì‚Å‚â‚ç‚È‚¢
class PlayerSlashUp : public PlayerAttackState
{
public:
	PlayerSlashUp(std::weak_ptr<Player> parent);
	~PlayerSlashUp();

	void Init() override;

private:

};

