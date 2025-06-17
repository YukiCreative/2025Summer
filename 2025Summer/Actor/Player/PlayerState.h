#pragma once
#include <memory>

class Player;

// �v���C���[�̏�Ԃ��N���X�ɕ�����
class PlayerState abstract : public std::enable_shared_from_this<PlayerState>
{
public:
	// �J�ڎ��̏��������̏�ԑ��ɏ�����̂���Ԃ̗��_����
	PlayerState(std::weak_ptr<Player> parent);
	virtual ~PlayerState();

	// �J�ڂ����Ƃ��A�J�ڂ�����Ԃ�Ԃ�
	// �����łȂ��Ƃ��͎�����Ԃ�
	virtual std::shared_ptr<PlayerState> Update() abstract;

protected:
	std::weak_ptr<Player> m_player;
};

