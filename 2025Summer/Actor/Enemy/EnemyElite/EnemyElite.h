#pragma once
#include "Enemy.h"
#include <random>

class EnemyEliteState;

class EnemyElite : public Enemy, public std::enable_shared_from_this<EnemyElite>
{
public:
	EnemyElite();

	void Init(const std::weak_ptr<Player> player, const Vector3& initPos, const int dupulicatedHandle);

	void UpdateState() override; 

	void CommitMove() override;

	void OnDeath() override;

	void OnCollisionEnter(std::shared_ptr<Actor> other) override;

	bool CheckAnimName(const std::string& animName) const;

	void BiteAttack();
	void ArmSwingAttack();
	Vector3 GetBiteRigPos() const;
	Vector3 GetArmRightStartPos() const;
	Vector3 GetArmRightEndPos() const;
	Vector3 GetArmLeftStartPos() const;
	Vector3 GetArmLeftEndPos() const;

	int GetAttackInterval();

private:

	static std::normal_distribution<> s_attackIntervalNormalDist;

	std::shared_ptr<EnemyEliteState> m_state;

private:

	void OnDamage(std::shared_ptr<AttackCol>) override;
};

