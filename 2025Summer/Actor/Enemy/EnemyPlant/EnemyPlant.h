#pragma once
#include "../Enemy.h"
#include <random>

class EnemyPlantState;

// ’e‚ğŒ‚‚Á‚Ä‚­‚é‚¼I
class EnemyPlant : public Enemy , public std::enable_shared_from_this<EnemyPlant>
{
public:
	EnemyPlant();

	void Init(const std::weak_ptr<Player> player, const Vector3& initPos, const int dupulicatedHandle);

	void UpdateState() override;

	void CommitMove() override;

	void SpawnBullet();
	void OnDeath() override;
	void OnDamage(std::weak_ptr<AttackCol> other);
	int GetAttackInterval();

private:

	static std::normal_distribution<> s_attackIntervalNormalDist;

	std::shared_ptr<EnemyPlantState> m_state;

private:
};

