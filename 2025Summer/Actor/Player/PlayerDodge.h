#pragma once
#include "PlayerState.h"

class PlayerDodge : public PlayerState
{
public:

	PlayerDodge(std::weak_ptr<Player> parent);
	~PlayerDodge();

	std::shared_ptr<PlayerState> Update() override;

private:

	int m_frame;
	bool m_isJustDodge;

	// ‰ñ”ğ‚ÆUŒ‚‚È‚ç‰ñ”ğ—Dæ
	bool m_isInputAheadAttack;
	bool m_isInputAheadDodge;

private:
	void JustDodge();
};

