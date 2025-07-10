#pragma once
#include "Actor.h"
#include <string>

class AnimationModel;
class EnemyState;
class Player;

// �G�̊�ꏈ��
class Enemy abstract : public Actor
{
public:
	// ���b�N�I���̐ݒ�
	Enemy();

	void Init(std::weak_ptr<Player> player, const Vector3& initPos);

	// �����ł͏������z�֐��͌p�����Ȃ�

	void ChangeAnim(const std::string& animName, const bool isLoop = true);

	void LookAtPlayer();
	Vector3 EnemyToPlayer() const;

protected:

	// ���f���͋��ʂŎ����Ă���
	std::shared_ptr<AnimationModel> m_model;

	// �v���C���[�̎Q�Ƃ���������
	std::weak_ptr<Player> m_player;

private:
};

