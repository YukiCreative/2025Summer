#pragma once
#include "EnemyEliteState.h"

class EnemyEliteGetUp : public EnemyEliteState
{
public:
	EnemyEliteGetUp(std::weak_ptr<EnemyElite> parent);
	~EnemyEliteGetUp();

	std::shared_ptr<EnemyEliteState> Update() override;

private:
};

