#pragma once
#include "EnemyEliteState.h"

class EnemyEliteDamage : public EnemyEliteState
{
public:
	EnemyEliteDamage(std::weak_ptr<EnemyElite> parent);
	~EnemyEliteDamage();

	std::shared_ptr<EnemyEliteState> Update() override;
};

