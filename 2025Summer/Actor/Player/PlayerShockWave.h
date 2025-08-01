#pragma once
#include "../AttackCol.h"

class EffekseerEffect;
class Player;

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

	void Init(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk, const float knockback,
		std::weak_ptr<Player> player);

	void Update() override;

	void Draw() const override;

	void CommitMove() override;

	void OnCollisionEnter(std::shared_ptr<Actor> other) override;

private:

	std::shared_ptr<DxLib::tagMATRIX> m_dir;
	std::weak_ptr<EffekseerEffect> m_effect;
	int m_frame;
	std::weak_ptr<Player> m_player;
	// 自身のY軸回転を記憶しておく
	float m_rotateY;
};

