#pragma once
#include "../AttackCol.h"

class PlayerAttackCol : public AttackCol, public std::enable_shared_from_this<PlayerAttackCol>
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

