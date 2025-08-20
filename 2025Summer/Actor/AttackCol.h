#pragma once
#include "Actor.h"

// UŒ‚—Í‚ğ‚ÂActor‚ªŒp³‚·‚é
class AttackCol abstract : public Actor
{
public:
	AttackCol() : Actor(false) {}

	void  SetAttackPower   (const float atk)  { m_attackPower = atk; }
	float GetAttackPower   () const           { return m_attackPower; }
	void SetKnockbackPower(const Vector3& pow){ m_knockbackPower = pow; }
	Vector3 GetKnockbackPower() const         { return m_knockbackPower; }
	void SetStunPower(const float& pow)		  { m_stunPower = pow; }
	float GetStunPower() const				  { return m_stunPower; }
protected:
	float m_attackPower = 0;
	Vector3 m_knockbackPower = Vector3::Zero();
	float m_stunPower = 0;
private:
};