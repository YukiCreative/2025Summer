#pragma once
#include <memory>

// �v���C���[�̏�Ԃ��N���X�ɕ�����
class PlayerState : public std::enable_shared_from_this<PlayerState>
{
public:
	// �J�ڎ��̏��������̏�ԑ��ɏ�����̂���Ԃ̗��_����
	PlayerState();
	virtual ~PlayerState();

	// �J�ڂ����Ƃ��A�J�ڂ�����Ԃ�Ԃ�
	// �����łȂ��Ƃ��͎�����Ԃ�
	virtual std::shared_ptr<PlayerState> Update() abstract;
};

