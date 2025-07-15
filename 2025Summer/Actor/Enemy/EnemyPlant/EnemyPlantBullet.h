#pragma once
#include "AttackCol.h"

namespace DxLib
{
	struct tagMATRIX;
}

class EffekseerEffect;

class EnemyPlantBullet : public AttackCol
{
public:
	EnemyPlantBullet();
	~EnemyPlantBullet();

	void Init(const Vector3& initPos, const DxLib::tagMATRIX& dir);

	void Update() override;

	void Draw() const override;

	void CommitMove() override;

	void OnCollisionEnter(std::shared_ptr<Actor> other) override;

private:

	int m_frame;
	std::shared_ptr<DxLib::tagMATRIX> m_dir;
	std::weak_ptr<EffekseerEffect> m_effect;
};

