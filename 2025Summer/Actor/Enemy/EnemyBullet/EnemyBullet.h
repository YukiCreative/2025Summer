#pragma once
#include "Enemy.h"

// �e�������Ă��邼�I
class EnemyBullet : public Enemy
{
public:
	EnemyBullet();

	void Init(const std::weak_ptr<Player> player, const Vector3& initPos);

	void Update() override;
	void Draw() const override;

	void CommitMove() override;

private:


};

