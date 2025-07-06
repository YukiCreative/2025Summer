#pragma once
#include "Actor.h"

class AnimationModel;
class Camera;
class PlayerState;
class ActorController;
class Image;
class PlayerSword;

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
	~Player();

	void Init(const std::weak_ptr<Camera> camera, std::weak_ptr<ActorController> cont);

	void Update() override;
	void Draw() const override;

	void OnCollision(std::shared_ptr<Actor> other) override;

	void CommitMove() override;

	// �g���Ă郂�f���̌���������Ȃ̂ł���p�ɏC�������s���Ԃ�
	DxLib::tagMATRIX GetModelMatrix() const;

	Vector3 GetRightInexPos() const;
	// �����̉E��̏��w�̃t���[�����l�����w�̃t���[���̃x�N�g��(�P��)
	Vector3 GetRightHandVec() const;

	// �����̂̌����ڂ�\������
	void EnableSword();
	// ���̓����蔻���L��������
	void EnableSwordCol(const float attackPower);

	void DisableSword();
	void DisableSwordCol();

public:
	// ������PlayerState�����Ō����������o�ϐ�
	std::shared_ptr<AnimationModel> m_model;
	std::weak_ptr<Camera> m_camera;
	std::shared_ptr<PlayerState> m_state;
	Vector3 m_targetPos;
	std::weak_ptr<Actor> m_lockOnActor;
	std::weak_ptr<ActorController> m_cont;
	Vector3 m_lockOnCursorPos;
	std::shared_ptr<Image> m_lockOnGraph;
	std::shared_ptr<PlayerSword> m_sword;
	// ���̃t���[���Ƀ��b�N�I���ΏۂƐG�ꂽ��true�ɂȂ�
	bool m_isContactLockOnActor;

public:
	// ������PlayerState�����Ō��Ăق����֐�
	void Move(const float moveSpeed);
	void MoveWithoutRotate(const float moveSpeed);
	void CameraMove();
	float DefaultGroundDrag();
	float DefaultAirDrag();
};

