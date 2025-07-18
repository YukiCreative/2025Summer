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

	void Init(std::weak_ptr<Player> player, const Vector3& initPos, const float initHP, const int dupulicatedHandle);

	// �����ł͏������z�֐��͌p�����Ȃ�

	void ChangeAnim(const std::string& animName, const bool isLoop = true);

	void LookAtPlayer(const float strength = 1.0f);
	Vector3 EnemyToPlayer() const;
	// ����̓��f����������Ԃ�Z-�����Ɍ����Ă���O��ł�
	Vector3 GetDir() const;
	DxLib::tagMATRIX GetModelMatrix() const;
	void SetInvincibility(const bool invincibleOrNot) { m_isInvincible = invincibleOrNot; }
	bool IsEndAnim() const;
	void AddVel(const Vector3& vel);

protected:

	HitPoint_t m_hitPoint;
	// ���f���͋��ʂŎ����Ă���
	std::shared_ptr<AnimationModel> m_model;
	// �v���C���[�̎Q�Ƃ���������
	std::weak_ptr<Player> m_player;
	bool m_isInvincible;

protected:

private:
};

