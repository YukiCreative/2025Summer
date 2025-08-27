#pragma once
#include "../AttackCol.h"

class PlayerAttackCol : public AttackCol
{
public:
	PlayerAttackCol() :
		m_isKnockUpAttack(false)
	{

	}

	void SetIsKnockUp(const bool knockUP) { m_isKnockUpAttack = knockUP; }
	bool IsKnockUpAttack() const { return m_isKnockUpAttack; }
private:

	// ‘Å‚¿ã‚°‘®«
	bool m_isKnockUpAttack;
};

