#pragma once
#include "EnemyEliteState.h"

class EnemyEliteStun : public EnemyEliteState
{
public:
	EnemyEliteStun(std::weak_ptr<EnemyElite> parent);
	~EnemyEliteStun();

	std::shared_ptr<EnemyEliteState> Update() override;
private:

};

