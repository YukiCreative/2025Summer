#pragma once
#include "AttackCol.h"

class Player;
class EffekseerEffect;

class PlayerSpecialAttackCol : public AttackCol
{
public:
	PlayerSpecialAttackCol();
	~PlayerSpecialAttackCol();

	void Init(std::weak_ptr<Player> parent, const Vector3& initPos);

	void Update() override;

	void Draw() const override;

	void CommitMove() override;

private:

	std::weak_ptr<Player> m_parent;

	std::weak_ptr<EffekseerEffect> m_effect;

	int m_frame;
};

