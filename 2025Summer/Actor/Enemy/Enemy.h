#pragma once
#include "Actor.h"
#include <string>
#include "RangeLimitedValue.h"
#include "HitPoint.h"

class AnimationModel;
class EnemyState;
class Player;

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

protected:

	HitPoint_t m_hitPoint;

	// ���f���͋��ʂŎ����Ă���
	std::shared_ptr<AnimationModel> m_model;

	// �v���C���[�̎Q�Ƃ���������
	std::weak_ptr<Player> m_player;

private:
};

