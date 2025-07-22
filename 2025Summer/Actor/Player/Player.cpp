#include "Player.h"
#include "ActorController.h"
#include "AnimationModel.h"
#include "Camera.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "Game.h"
#include "Image.h"
#include "Input.h"
#include "PlayerDeath.h"
#include "PlayerIdle.h"
#include "PlayerMiddleDamage.h"
#include "PlayerNormal.h"
#include "PlayerShockWave.h"
#include "PlayerState.h"
#include "PlayerSword.h"
#include "Rigid.h"
#include "SphereCollider.h"
#include "PlayerSpecialAttackCol.h"
#include <DxLib.h>

namespace
{
	constexpr float kDrag = 0.2f;

	constexpr int kWeight = 10;

	constexpr float kCameraHSpeed = 0.00001f;
	constexpr float kCameraVSpeed = 0.00001f;

	const Vector3 kCapsuleEndPosOffset = {0, 200, 0};
	constexpr float kCapsuleRadius = 30;
	constexpr float kAnimPlaySpeed = 30.0f;

	const std::string kRightIndexFrame = "mixamorig:RightHandIndex1";
	const std::string kRightPinkyFrame = "mixamorig:RightHandPinky1";


	// �Ƃ肠�����ߋ�1�b�̓��͗������c��
	constexpr int kInputHistoryMax = 60;

	constexpr float kLockOnRotateSpeed = 0.2f;
}

Player::Player() :
	m_targetPos(),
	Actor(false),
	m_isContactLockOnActor(false),
	m_isInvincible(false),
	m_canLockOn(true),
	m_isDraw(true)
{
}

Player::~Player()
{
}

void Player::Init(const std::weak_ptr<Camera> camera, std::weak_ptr<ActorController> cont)
{
	m_kind = ActorKind::kPlayer;

	m_camera = camera;
	m_cont = cont;

	m_model = std::make_shared<AnimationModel>();
	m_model->Init("Data/Model/Player.mv1", kAnimPlaySpeed);

	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + kCapsuleEndPosOffset, kWeight, false, false, kCapsuleRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_sword = std::make_shared<PlayerSword>();
	m_sword->Init(weak_from_this());
	m_cont.lock()->AddActor(m_sword);

	m_state = std::make_shared<PlayerNormal>(weak_from_this(), std::make_shared<PlayerIdle>(weak_from_this()));

	m_hp.SetMax();
	m_gauge.SetMin();
}

void Player::Update()
{
	// ��Ԃ��X�V
	// �؂�ւ������Ⴄ�C���X�^���X�������Ă���
	m_state = m_state->Update();

	m_model->Update();

	// �t���O�����Z�b�g
	m_isContactLockOnActor = false;
}

void Player::CameraMove()
{
	// �J�����𓮂���
	Vector2 rightAxis = Input::GetInstance().GetRightInputAxis();
	rightAxis.y *= -1;

	m_camera.lock()->RotateCameraUpVecY(rightAxis.x * kCameraHSpeed);
	m_camera.lock()->RotateCameraV(rightAxis.y * kCameraVSpeed);
}

void Player::SetInputDir(const PlayerInputDir& dir)
{
	// ����������Ȃ�A�폜
	if (m_inputList.size() >= kInputHistoryMax)
	{
		m_inputList.pop_front();
	}

	m_inputList.emplace_back(dir);
}

void Player::SpawnShockWave(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk, const float knockback)
{
	auto shockWave = std::make_shared<PlayerShockWave>();
	shockWave->Init(rot, initPos, atk, knockback);
	SpawnActor(shockWave);
}

void Player::OnDamage(std::shared_ptr<AttackCol> attack)
{
	if (m_isInvincible) return;

	// HP���炷
	m_hp -= attack->GetAttackPower();

#if _DEBUG
	printf("�_���[�W%f", attack->GetAttackPower());
#endif

	// �m�b�N�o�b�N
	m_collidable->AddVel((m_pos - attack->GetPos()).GetNormalize() * attack->GetKnockbackPower());

	if (m_hp.IsMin())
	{
		// �����I
		m_state->SetStateDeath();
	}
	else
	{
		m_state->SetStateDamage();
	}
}

void Player::LockOnRotate()
{
	// �v���C���[��G�����ɉ�]
	auto lockOnPosXZ = m_lockOnActor.lock()->GetPos().XZ();
	auto posXZ = GetPos().XZ();

	auto playerToLockOnXZ = (lockOnPosXZ - posXZ).GetNormalize();

	auto playerDir = m_model->GetDirection();

	auto dot = playerToLockOnXZ.Dot(playerDir);

	float rot = playerDir.Cross(playerToLockOnXZ).y * kLockOnRotateSpeed;

	// ���傤�ǐ^���΂Ɍ����Ă����ꍇ�̏���
	if (dot < -0.9999f && rot < 0.0001f)
	{
		rot += 0.1f;
	}

	m_model->RotateUpVecY(rot);
}

void Player::ReleaseLockOn()
{
	// ����
	m_lockOnActor.reset();
}


void Player::Move(const float moveSpeed)
{
	// ���͂ňړ�
	Vector2 inputAxis = Input::GetInstance().GetLeftInputAxis();
	inputAxis.x *= -1;		// ���E(�������͏㉺)�����]���Ă���̂Œ���
	// �A�i���O���͂𐶂��������̂Ő��K���͂��Ȃ�

	Vector3 vel = Vector3{ inputAxis.x, 0,inputAxis.y } * moveSpeed;
	// �ړ��ʂ��J�����̌����ɕ␳
	vel = m_camera.lock()->RotateVecToCameraDirXZ(vel, Vector3::Back());

	// �v���C���[�̌��݂̌������擾
	const Vector3 dir = m_model->GetDirection();

	// ���x�ƌ����Ƃ̊O�ς��Ƃ��āA�o���l�ŉ�]
	Vector3 cross = dir.Cross(vel);
	const float dot = dir.Dot(vel);

	// �^���΂Ɉړ����悤�Ƃ���ƁA�������o�O��̂œ��ʂɏ���
	if (cross.y <= 0.01f && dot <= -0.9999f)
	{
		// �����I�ɉ�
		cross.y += 0.5f;
	}

	m_model->RotateUpVecY(cross.y * 0.3f);

	const float clampDot = std::max(dot, 0.0f);

	// �����̌����ƈړ��������������Ă���Ƃ��A���̕����Ɉړ����Ȃ�
	m_collidable->AddVel(vel * clampDot);
}

void Player::MoveWithoutRotate(const float moveSpeed)
{
	// ���̖��̒ʂ��]���Ȃ�Move
	// �����ɂ��ړ��������s��Ȃ�

		// ���͂ňړ�
	Vector2 inputAxis = Input::GetInstance().GetLeftInputAxis();
	inputAxis.x *= -1;		// ���E(�������͏㉺)�����]���Ă���̂Œ���
	// �A�i���O���͂𐶂��������̂Ő��K���͂��Ȃ�

	Vector3 vel = Vector3{ inputAxis.x, 0,inputAxis.y } *moveSpeed;
	// �ړ��ʂ��J�����̌����ɕ␳
	vel = m_camera.lock()->RotateVecToCameraDirXZ(vel, Vector3::Back());

	m_collidable->AddVel(vel);
}

void Player::Draw() const
{
	if (m_isDraw)
	{
		m_model->Draw();
	}

#if _DEBUG
	m_collidable->GetCol().Draw();

	if (!m_lockOnActor.expired())
	{
		DrawLine3D(m_pos + kCapsuleEndPosOffset, m_lockOnActor.lock()->GetPos(), 0xfffffff);
	}

	DrawSphere3D(m_targetPos, 10, 10, 0xffffff, 0xffffff, true);
#endif
}

void Player::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	m_isContactLockOnActor = (other == m_lockOnActor.lock());

	if (other->GetKind() == ActorKind::kEnemyAttack)
	{
		auto attack = std::static_pointer_cast<AttackCol>(other);

		OnDamage(attack);
	}
}

void Player::CommitMove()
{
	// ���x���������Ď擾
	const Vector3 vel = m_collidable->UpdateRigid();
	m_pos += vel;
	m_collidable->SetPos(m_pos + kCapsuleEndPosOffset * 0.5f);

	m_camera.lock()->SetTargetPos(m_targetPos);
	m_model->SetPos(m_pos);
}

DxLib::tagMATRIX Player::GetModelMatrix() const
{
	auto mat = m_model->GetMatrix();

	mat.m[0][0] *= -1;
	mat.m[0][1] *= -1;
	mat.m[0][2] *= -1;

	mat.m[2][0] *= -1;
	mat.m[2][1] *= -1;
	mat.m[2][2] *= -1;

	return mat;
}

Vector3 Player::GetRightInexPos() const
{
	return m_model->GetFramePosition(kRightIndexFrame);
}

Vector3 Player::GetRightHandVec() const
{
	const Vector3 rightIndex = m_model->GetFramePosition(kRightIndexFrame);
	const Vector3 rightPinky = m_model->GetFramePosition(kRightPinkyFrame);

	return rightIndex - rightPinky;
}

void Player::EnableSword()
{
	m_sword->Enable();
}

void Player::EnableSwordCol(const float attackPower, const float knockbackPower)
{
	m_sword->ColEnable();
	m_sword->SetAttackPower(attackPower);
	m_sword->SetKnockbackPower(knockbackPower);
}

void Player::DisableSword()
{
	m_sword->Disable();
}

void Player::DisableSwordCol()
{
	m_sword->ColDisable();
}

Vector3 Player::GetLockOnActorScreenPos() const
{
	if (m_lockOnActor.expired()) return Vector3::Zero();

	return ConvWorldPosToScreenPos(m_lockOnActor.lock()->GetPos());
}

float Player::GetHpRatio() const
{
	return m_hp.GetRatio();
}

void Player::ChangeAnim(const std::string& animName, const bool loopOrNot)
{
	m_model->ChangeAnimation(animName, loopOrNot);
}

void Player::Disappear()
{
	m_isDraw = false;
}

void Player::Apeear()
{
	m_isDraw = true;
}

void Player::SpecialAttack()
{
	auto special = std::make_shared<PlayerSpecialAttackCol>();
	special->Init(weak_from_this(), m_pos);
	SpawnActor(special);
}
