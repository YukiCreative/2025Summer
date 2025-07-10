#pragma once
#include "Actor.h"

// UŒ‚—Í‚ğ‚ÂActor‚ªŒp³‚·‚é
class AttackCol abstract : public Actor
{
public:
	AttackCol() : Actor(false) {}

	void  SetAttackPower   (const float atk) { m_attackPower = atk; }
	float GetAttackPower   () const          { return m_attackPower; }
	void SetKnockbackPower(const float pow)  { m_knockbackPower = pow; }
	float GetKnockbackPower() const          { return m_knockbackPower; }
protected:
	float m_attackPower = 0;
	float m_knockbackPower = 0;
private:
};