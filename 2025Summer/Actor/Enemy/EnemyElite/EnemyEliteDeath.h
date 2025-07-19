#pragma once
#include "EnemyEliteState.h"


class EnemyEliteDeath : public EnemyEliteState
{
public:
	EnemyEliteDeath(std::weak_ptr<EnemyElite> parent);
	~EnemyEliteDeath();

	std::shared_ptr<EnemyEliteState> Update() override;
};

