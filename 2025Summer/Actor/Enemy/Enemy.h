#pragma once
#include "Actor.h"
#include <string>
#include "RangeLimitedValue.h"
#include "HitPoint.h"

class AnimationModel;
class EnemyState;
class Player;
class AttackCol;

namespace DxLib
{
	struct tagMATRIX;
}

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
	DxLib::tagMATRIX GetModelMatrix() const;
	// true�Ŗ��G
	void SetInvincibility(const bool invincibleOrNot) { m_isInvincible = invincibleOrNot; }
	bool IsEndAnim() const;
	void AddVel(const Vector3& vel);

	virtual void OnDeath() abstract;
	// �ꎞ�Ifalse
	void StartDissolve() { m_isDissolving = false; }

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
	// �v���C���[�̎Q�Ƃ���������
	std::weak_ptr<Player> m_player;
	bool m_isInvincible;
	int m_cBuffH;
	EnemyCBuff* m_cBuff;
	// �Â񂶂Ă��ׂĂ�Enemy�Ń��[�h���悤
	int m_dissolvePsH;
	int m_dissolveVsH;
	int m_dissolveTex;

	bool m_isDissolving;

protected:

	virtual void OnDamage(std::shared_ptr<AttackCol>) abstract;

private:

	void InitDissolve();
	// ���S���oDraw
	void UpdateDissolve();
	void DissolveDraw() const;

};