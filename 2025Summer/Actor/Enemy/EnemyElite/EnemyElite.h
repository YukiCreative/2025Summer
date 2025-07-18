#pragma once
#include "Enemy.h"

class EnemyEliteState;

class EnemyElite : public Enemy, public std::enable_shared_from_this<EnemyElite>
{
public:
	EnemyElite();

	void Init(const std::weak_ptr<Player> player, const Vector3& initPos, const int dupulicatedHandle);

	void Update() override;
	void Draw() const override;

	void CommitMove() override;

	void OnCollisionEnter(std::shared_ptr<Actor> other) override;
private:

	std::shared_ptr<EnemyEliteState> m_state;
};

