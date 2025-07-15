#pragma once
#include <memory>
#include "Vector3.h"

class Player;

// PlayerState�ŋ��L�������萔�͂����ɓ���悤
namespace
{
	constexpr float kMoveThreshold = 0.001f;
	const Vector3 kCameraTargetOffset = { 0,150,0 };
	constexpr float kLockOnWalkSpeed = 0.002f;
}

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

protected:
};

