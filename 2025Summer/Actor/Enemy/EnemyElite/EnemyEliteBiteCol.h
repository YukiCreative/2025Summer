#pragma once
#include "../../AttackCol.h"

class EnemyElite;
class EffekseerEffect;

class EnemyEliteBiteCol : public AttackCol
{
public:
	EnemyEliteBiteCol();
	~EnemyEliteBiteCol();

	void Init(std::weak_ptr<EnemyElite> parent);

	void Update() override;

	void Draw() const override;

	void CommitMove() override;

private:

	std::weak_ptr<EnemyElite> m_parent;

	std::weak_ptr<EffekseerEffect> m_effect;

	int m_frame;
};

