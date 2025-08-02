#pragma once
#include "../../AttackCol.h"

class EnemyBug;
class EffekseerEffect;

// プレイヤーの方式と異なり、攻撃時に出現して攻撃が終わったら消す
class EnemyBugAttackCol : public AttackCol
{
public:
	EnemyBugAttackCol();
	~EnemyBugAttackCol();

	void Init(std::weak_ptr<EnemyBug> parent);

	void Update() override;

	void Draw() const override;

	void CommitMove() override;

private:

	std::weak_ptr<EnemyBug> m_parent;

	std::weak_ptr<EffekseerEffect> m_effect;

	int m_frame;
};

