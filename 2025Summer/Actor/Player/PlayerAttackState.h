#pragma once
#include "PlayerState.h"
#include <string>

class PlayerAttackState : public PlayerState
{
public:
	PlayerAttackState(std::weak_ptr<Player> parent);
	~PlayerAttackState();

	// �R���{��̃C���X�^���X�������Ă���֌W��A
	// �������̃^�C�~���O��C�ӂɂ��Ȃ��Ƃ����Ȃ��Ȃ���
	virtual void Init() abstract;

	std::shared_ptr<PlayerState> Update() override;

private:

	bool m_isEnterAttack;
	int m_frame;

	// �����̕ϐ���h���N���X�ŏ��������Ă��炤
	std::string m_animName;
	int m_enableAttackColFrame;
	int m_disableAttackColFrame;
	int m_stateTotalFrame;
	// �G�ւ̋z����
	int m_trackFrame;
	float m_trackForce;
	int m_acceptNextAttackInputFrame;
	int m_enableComboFrame;
	float m_attackPower;
	// �h������U��
	std::shared_ptr<PlayerAttackState> m_comboAttack;

private:

	// �h����ŔC�ӂ̏����������������Ƃ��Ɏg��
	virtual void OptionalProcess() {}
};

