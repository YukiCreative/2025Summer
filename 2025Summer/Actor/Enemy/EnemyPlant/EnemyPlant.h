#pragma once
#include "Enemy.h"

class EnemyPlantState;

// ’e‚ğŒ‚‚Á‚Ä‚­‚é‚¼I
class EnemyPlant : public Enemy
{
public:
	EnemyPlant();

	void Init(const std::weak_ptr<Player> player, const Vector3& initPos);

	void Update() override;
	void Draw() const override;

	void CommitMove() override;

private:

	std::shared_ptr<EnemyPlantState> m_state;
};

