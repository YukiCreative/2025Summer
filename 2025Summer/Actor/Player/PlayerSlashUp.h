#pragma once
#include "PlayerAttackState.h"

// �v���C���[�̍U����(PlayerAttackState�݂�����)�����𑩂˂�ꂻ�������ǎ��s�������Ȃ̂ł��Ȃ�
class PlayerSlashUp : public PlayerAttackState
{
public:
	PlayerSlashUp(std::weak_ptr<Player> parent);
	~PlayerSlashUp();

	void Init() override;

private:

};

