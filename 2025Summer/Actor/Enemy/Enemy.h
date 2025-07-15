#pragma once
#include "Actor.h"
#include <string>
#include "RangeLimitedValue.h"
#include "HitPoint.h"

class AnimationModel;
class EnemyState;
class Player;
class AttackCol;

// �G�̊�ꏈ��
class Enemy abstract : public Actor
{
public:
	// ���b�N�I���̐ݒ�
	Enemy();

	void Init(std::weak_ptr<Player> player, const Vector3& initPos, const float initHP);

	// �����ł͏������z�֐��͌p�����Ȃ�

	void ChangeAnim(const std::string& animName, const bool isLoop = true);

	void LookAtPlayer();
	Vector3 EnemyToPlayer() const;

	virtual void OnDeath() abstract;

protected:

	HitPoint_t m_hitPoint;
	// ���f���͋��ʂŎ����Ă���
	std::shared_ptr<AnimationModel> m_model;
	// �v���C���[�̎Q�Ƃ���������
	std::weak_ptr<Player> m_player;
	bool m_isInvincible;

protected:

	virtual void OnDamage(std::shared_ptr<AttackCol> other) abstract;

private:
};

