#pragma once
#include "PlayerState.h"

// �v���C���[�Ə�Ԃ̊ԂɈꖇ����Ă���N���X
// ���̂Ƃ��냍�b�N�I�����Ă邩�����łȂ����Ɏg���Ă�
// ���[���K�w�\�����K�v�ɂȂ������͐݌v��ύX����K�v������
class PlayerIntermediateState : public std::enable_shared_from_this<PlayerIntermediateState>
{
public:
	PlayerIntermediateState(std::weak_ptr<Player> parent);
	~PlayerIntermediateState();

	// ���ꂪ�~��������
	// �_���[�W��Ԃ����荞�݂Ŏ��s����
	void SetStateDamage();
	// ���S
	void SetStateDeath();

	virtual std::shared_ptr<PlayerIntermediateState> Update() abstract;

protected:

	// Update���ŕK�����s����悤��
	void UpdateChildState();
	 
protected:

	std::shared_ptr<PlayerState> m_childState;
	std::weak_ptr<Player> m_player;

private:


};

