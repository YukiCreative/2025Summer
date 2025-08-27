#pragma once
#include "EnemyEliteState.h"

class EnemyEliteKnockUp : public EnemyEliteState
{
public:
	EnemyEliteKnockUp(std::weak_ptr<EnemyElite> parent);
	~EnemyEliteKnockUp();

	std::shared_ptr<EnemyEliteState> Update() override;

private:

};

