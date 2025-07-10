#pragma once
#include "AttackCol.h"

class EffekseerEffect;

namespace DxLib
{
	struct tagMATRIX;
}

// 中身は当たり判定とエフェクト
class PlayerShockWave : public AttackCol
{
public:

	PlayerShockWave();
	~PlayerShockWave();

	void Init(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk);

	void Update() override;

	void Draw() const override;

	void CommitMove() override;

	void OnCollisionEnter(std::shared_ptr<Actor> other) override;

private:

	std::shared_ptr<DxLib::tagMATRIX> m_dir;
	std::shared_ptr<EffekseerEffect> m_effect;
	int m_frame;
};

