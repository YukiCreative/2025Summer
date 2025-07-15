#pragma once
#include "PlayerState.h"
#include <string>

class PlayerAttackState abstract : public PlayerState 
{
public:
	PlayerAttackState(std::weak_ptr<Player> parent);
	~PlayerAttackState();

	// �R���{��̃C���X�^���X�������Ă���֌W��A
	// �������̃^�C�~���O��C�ӂɂ��Ȃ��Ƃ����Ȃ��Ȃ���
	virtual void Init() abstract;
	void PlayAnim();

	std::shared_ptr<PlayerState> Update() override final;

protected:

	bool m_isEnterAttack;
	int m_frame;

	// �����̕ϐ���h���N���X�ŏ��������Ă��炤
	std::string m_animName;
	bool m_isLoopAnim;
	int m_enableAttackColFrame;
	int m_disableAttackColFrame;
	int m_stateTotalFrame;
	// �G�ւ̋z����
	int m_trackFrame;
	float m_trackForce;
	float m_enemyTrackForce;
	int m_acceptNextAttackInputFrame;
	int m_enableComboFrame;
	float m_attackPower;
	float m_knockbackPower;
	// �h������U��
	std::shared_ptr<PlayerAttackState> m_comboAttack;

protected:

	// �h����ŔC�ӂ̏����������������Ƃ��Ɏg��
	virtual void OptionalProcess() {}

	// �G�ɋz�����������������܂�
	// �z�����Ƃ��Ƃ����łȂ�(�C�ӓ��́A�ΏۂȂ�)���ŉ�����͂�ς�����
	Vector3 TrackingVec(const float strength, const float trackEnemyStrength);
};

