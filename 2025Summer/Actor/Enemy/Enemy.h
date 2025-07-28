#pragma once
#include "Actor.h"
#include <string>
#include "RangeLimitedValue.h"
#include "HitPoint.h"
#include "EnemyKind.h"

class AnimationModel;
class EnemyState;
class Player;
class AttackCol;

namespace DxLib
{
	struct tagMATRIX;
}

class EffekseerEffect;

// �G�̊�ꏈ��
class Enemy abstract : public Actor
{
public:
	// ���b�N�I���̐ݒ�
	Enemy();
	~Enemy();

	void Init(std::weak_ptr<Player> player, const Vector3& initPos, const float initHP, const int dupulicatedHandle);

	void Update() override final;
	virtual void UpdateState() abstract;

	void Draw() const override final;

	void ChangeAnim(const std::string& animName, const bool isLoop = true);

	void LookAtPlayer(const float strength = 1.0f);
	Vector3 EnemyToPlayer() const;
	// ����̓��f����������Ԃ�Z-�����Ɍ����Ă���O��ł�
	Vector3 GetDir() const;
	void RotateAxisY(const float rad);
	DxLib::tagMATRIX GetModelMatrix() const;
	// true�Ŗ��G
	void SetInvincibility(const bool invincibleOrNot) { m_isInvincible = invincibleOrNot; }
	bool IsEndAnim() const;
	void AddVel(const Vector3& vel);

	virtual void OnDeath() abstract;
	void StartBloodEffect();

	EnemyKind GetEnemyKind() const { return m_enemyKind; }

	// ���g�����b�N�I���s�ɂ���
	void DisableLockOn();

protected:

	// ���f���f�B�]���u
	struct EnemyCBuff
	{
		// 0�`1
		float time;
		float dammy[7];
	};

	HitPoint_t m_hitPoint;
	// ���f���͋��ʂŎ����Ă���
	std::shared_ptr<AnimationModel> m_model;
	EnemyKind m_enemyKind;
	// �v���C���[�̎Q�Ƃ���������
	std::weak_ptr<Player> m_player;
	bool m_isInvincible;

	std::weak_ptr<EffekseerEffect> m_bloodEffect;

	// ���S���̌����Ԃ��G�t�F�N�g�̔��������ǂ̃��O�ɂ��邩
	// �h���N���X�Őݒ肵�Ă��炤
	int m_bloodFrameIndex;

protected:

	virtual void OnDamage(std::shared_ptr<AttackCol>) abstract;

private:

};