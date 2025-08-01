#pragma once
#include "../AttackCol.h"

class EffekseerEffect;
class Player;

namespace DxLib
{
	struct tagMATRIX;
}

// ���g�͓����蔻��ƃG�t�F�N�g
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
	// ���g��Y����]���L�����Ă���
	float m_rotateY;
};

