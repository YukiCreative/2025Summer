#pragma once
#include "Actor.h"
#include <list>
#include "PlayerState.h"
#include "RangeLimitedValue.h"

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

enum class PlayerInputDir
{
	kFront,
	kRight,
	kLeft,
	kBack
};

namespace
{
	constexpr float kMinHP = 0.0f;
	constexpr float kMaxHP = 99990.0f;
}

using PlayerHP = RangeLimitedValue<float, kMinHP, kMaxHP>;

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
	bool m_isContactLockOnActor;
	// �v���C���[�̈ړ������̗���
	// �R�}���h�Ɏg�p
	std::list<PlayerInputDir> m_inputList;
	PlayerHP m_hp;

public:
	// ������PlayerState�����Ō��Ăق����֐�
	void Move(const float moveSpeed);
	void MoveWithoutRotate(const float moveSpeed);
	void CameraMove();
	float DefaultGroundDrag();
	float DefaultAirDrag();
	void SetInputDir(const PlayerInputDir& dir);
	// PlayerShockWaveSlash�p
	void SpawnShockWave(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk);
	void OnDamage(const float damage);
	// �v���C���[�����b�N�I���Ώۂ֌�����
	void LockOnRotate();
};