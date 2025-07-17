#pragma once
#include "Actor.h"
#include <list>
#include "PlayerIntermediateState.h"
#include "RangeLimitedValue.h"

class AnimationModel;
class Camera;
class ActorController;
class Image;
class PlayerSword;
class AttackCol;

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
	constexpr float kMaxHP = 3000.0f;
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

	void OnCollisionEnter(std::shared_ptr<Actor> other) override;

	void CommitMove() override;

	// �g���Ă郂�f���̌���������Ȃ̂ł���p�ɏC�������s���Ԃ�
	DxLib::tagMATRIX GetModelMatrix() const;

	Vector3 GetRightInexPos() const;
	// �����̉E��̏��w�̃t���[�����l�����w�̃t���[���̃x�N�g��(�P��)
	Vector3 GetRightHandVec() const;

	// �����̂̌����ڂ�\������
	void EnableSword();
	// ���̓����蔻���L��������
	void EnableSwordCol(const float attackPower, const float knockbackPower = 0.0f);

	void DisableSword();
	void DisableSwordCol();
	void SetInvincibility(const bool invincibleOrNot) { m_isInvincible = invincibleOrNot; }
	bool IsInvincible() const { return m_isInvincible; }

	Vector3 GetLockOnActorScreenPos() const;
	bool IsLockOn() const { return !m_lockOnActor.expired(); }
	bool CanLockOn() const { return m_canLockOn; }
	float GetHpRatio() const;

public:
	// ������PlayerState�����Ō����������o�ϐ�
	std::shared_ptr<AnimationModel> m_model;
	std::weak_ptr<Camera> m_camera;
	std::shared_ptr<PlayerIntermediateState> m_state;
	Vector3 m_targetPos;
	std::weak_ptr<Actor> m_lockOnActor;
	std::weak_ptr<ActorController> m_cont;
	std::shared_ptr<PlayerSword> m_sword;
	bool m_isContactLockOnActor;
	// �v���C���[�̈ړ������̗���
	// �R�}���h�Ɏg�p
	std::list<PlayerInputDir> m_inputList;
	PlayerHP m_hp;
	bool m_isInvincible;
	bool m_canLockOn;

public:
	// ������PlayerState�����Ō��Ăق����֐�
	void Move(const float moveSpeed);
	void MoveWithoutRotate(const float moveSpeed);
	void CameraMove();
	void SetInputDir(const PlayerInputDir& dir);
	// PlayerShockWaveSlash�p
	void SpawnShockWave(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk, const float knockback);
	void OnDamage(std::shared_ptr<AttackCol> attack);
	// �v���C���[�����b�N�I���Ώۂ֌�����
	void LockOnRotate();
	void ReleaseLockOn();
	void SetCanLockOn(const bool canOrNot) { m_canLockOn = canOrNot; }
};