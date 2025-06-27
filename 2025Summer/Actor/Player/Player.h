#pragma once
#include "Actor.h"

class AnimationModel;
class Camera;
class PlayerState;
class ActorController;
class Image;
class VirtualCamera;
class RotateCamera;

namespace DxLib
{
	struct tagMATRIX;
}


// �A�N�Z�X���x����public�ɂȂ��Ă���̂́A�X�e�[�g�N���X�ɏ��𖾂��n������
// ���p���ցI
class Player : public Actor, public std::enable_shared_from_this<Player>
{
public:
	// �O�����猩�Ă������֐�

	Player();

	void Init(std::weak_ptr<ActorController> cont);

	void Update() override;
	void Draw() const override;

	void OnCollision(std::shared_ptr<Actor> other) override;

	void CommitMove() override;

	// �g���Ă郂�f���̌���������Ȃ̂ł���p�ɏC�������s���Ԃ�
	DxLib::tagMATRIX GetModelMatrix() const;

public:
	// ������PlayerState�����Ō����������o�ϐ�
	std::shared_ptr<AnimationModel> m_model;
	std::shared_ptr<PlayerState> m_state;
	Vector3 m_targetPos;
	std::weak_ptr<Actor> m_lockOnActor;
	std::weak_ptr<ActorController> m_cont;
	Vector3 m_lockOnCursorPos;
	std::shared_ptr<Image> m_lockOnGraph;

	std::shared_ptr<RotateCamera> m_rotateCamera;

public:
	// ������PlayerState�����Ō��Ăق����֐�
	void Move(const float moveSpeed);
	void MoveWithoutRotate(const float moveSpeed);
	void CameraMove();
	float DefaultGroundDrag();
	float DefaultAirDrag();
	void LockOn();
};

