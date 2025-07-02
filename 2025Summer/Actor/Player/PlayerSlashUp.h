#pragma once
#include "PlayerState.h"

// �v���C���[�̍U����(PlayerAttackState�݂�����)�����𑩂˂�ꂻ�������ǎ��s�������Ȃ̂ł��Ȃ�
class PlayerSlashUp : public PlayerState
{
public:
	PlayerSlashUp(std::weak_ptr<Player> parent);
	~PlayerSlashUp();

	std::shared_ptr<PlayerState> Update() override;

private:

	bool m_isEnterAttack;
	int m_frame;
};

