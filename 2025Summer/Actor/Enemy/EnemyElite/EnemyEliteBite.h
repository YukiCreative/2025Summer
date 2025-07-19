#pragma once
#include "EnemyEliteState.h"

class EnemyEliteBite : public EnemyEliteState
{
public:
	EnemyEliteBite(std::weak_ptr<EnemyElite> parent);
	~EnemyEliteBite();

	std::shared_ptr<EnemyEliteState> Update() override;
};

