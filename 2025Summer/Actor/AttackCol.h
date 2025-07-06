#pragma once
#include "Actor.h"

// �U���͂�����Actor���p������
class AttackCol abstract : public Actor
{
public:
	AttackCol() : Actor(false) {}

	void  SetAttackPower(const float atk) { m_attackPower = atk; }
	float GetAttackPower() const { return m_attackPower; }
protected:
	float m_attackPower = 0;
private:
};