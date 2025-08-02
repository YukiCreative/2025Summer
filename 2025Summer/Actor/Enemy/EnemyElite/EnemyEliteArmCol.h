#pragma once
#include "../../AttackCol.h"

class EnemyElite;
class EffekseerEffect;

enum class LeftOrRight
{
	kLeft,
	kRight,
};

class EnemyEliteArmCol : public AttackCol
{
public:
	EnemyEliteArmCol();
	~EnemyEliteArmCol();

	void Init(std::weak_ptr<EnemyElite> parent, const LeftOrRight LR);

	void Update() override;

	void Draw() const override;

	void CommitMove() override;

private:

	std::weak_ptr<EnemyElite> m_parent;

	std::weak_ptr<EffekseerEffect> m_effect;

	int m_frame;

	LeftOrRight m_LR;
	Vector3 m_endPos;

private:

	void SetCapsulePos();
};

