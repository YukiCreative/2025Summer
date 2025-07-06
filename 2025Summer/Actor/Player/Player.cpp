#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"
#include "Collidable.h"
#include "Rigid.h"
#include "CapsuleCollider.h"
#include "Camera.h"
#include <DxLib.h>
#include "PlayerState.h"
#include "PlayerNormal.h"
#include "ActorController.h"
#include "Game.h"
#include "Image.h"
#include "PlayerSword.h"
#include "PlayerShockWave.h"

namespace
{
	const PhysicalMaterial kPhysiMat =
	{
		0.2f, 0.05f, 0.1f
	};

	constexpr int kWeight = 10;

	constexpr float kCameraHSpeed = 0.00001f;
	constexpr float kCameraVSpeed = 0.00001f;

	const Vector3 kCapsuleEndPosOffset = {0, 200, 0};
	constexpr float kCapsuleRadius = 30;
	constexpr float kAnimPlaySpeed = 30.0f;

	const std::string kLockOnCursorFile = "LockOnCursor.png";

	const std::string kRightIndexFrame = "mixamorig:RightHandIndex1";
	const std::string kRightPinkyFrame = "mixamorig:RightHandPinky1";

	constexpr float kFieldRadius = 2000.0f;

	// �Ƃ肠�����ߋ�1�b�̓��͗������c��
	constexpr int kInputHistoryMax = 60;
}

Player::Player() :
	m_targetPos(),
	Actor(false),
	m_isContactLockOnActor(false)
{
}

Player::~Player()
{
}

void Player::Init(const std::weak_ptr<Camera> camera, std::weak_ptr<ActorController> cont)
{
	m_camera = camera;
	m_cont = cont;

	m_model = std::make_shared<AnimationModel>();
	m_model->Init("Data/Model/Player.mv1", kAnimPlaySpeed);

	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + kCapsuleEndPosOffset, kWeight, false, false, kCapsuleRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysiMat);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_lockOnGraph = std::make_shared<Image>();
	m_lockOnGraph->Init(kLockOnCursorFile);

	m_sword = std::make_shared<PlayerSword>();
	m_sword->Init(weak_from_this());
	m_cont.lock()->AddActor(m_sword);

	m_state = std::make_shared<PlayerNormal>(weak_from_this());
}

void Player::Update()
{
	// ��Ԃ��X�V
	// �؂�ւ������Ⴄ�C���X�^���X�������Ă���
	m_state = m_state->Update();

	// �ړ��𐧌�
	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	const Vector3 nextPos = m_pos + m_collidable->GetVel();

	// �ړ�����̃G���A����o�Ă�����
	if (nextPos.SqrMagnitude() > kFieldRadius * kFieldRadius)
	{
		// �����j�~����悤�Ɉړ����x��ς�����

		// �␳��̈ʒu
		const Vector3 radiusDir = nextPos.GetNormalize() * kFieldRadius;

		// ���x�ɂ��Đݒ�
		m_collidable->SetVel(radiusDir - m_pos);
	}

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

float Player::DefaultGroundDrag()
{
	return kPhysiMat.groundDrag.Value();
}

float Player::DefaultAirDrag()
{
	return kPhysiMat.airDrag.Value();
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

void Player::SpawnShockWave(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk)
{
	auto shockWave = std::make_shared<PlayerShockWave>();
	shockWave->Init(rot, initPos, atk);
	SpawnActor(shockWave);
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
		cross.y += 0.1f;
	}

	m_model->RotateUpVecY(cross.y * 0.3f);

	const float clampDot = max(dot, -0.1f);

	// �����̌����ƈړ��������������Ă���ƁA�ړ��ʂ����]����
	// �U������ۂɈ�u���Ε����Ɉړ�����݂�����
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
	m_model->Draw();

	// ���b�N�I���`��
	if (!m_lockOnActor.expired())
	{
		m_lockOnGraph->Draw({ m_lockOnCursorPos.x, m_lockOnCursorPos.y });
	}

#if _DEBUG
	m_collidable->GetCol().Draw();

	if (!m_lockOnActor.expired())
	{
		DrawLine3D(m_pos + kCapsuleEndPosOffset, m_lockOnActor.lock()->GetPos(), 0xfffffff);
	}

	auto m = GetModelMatrix();

	Vector3 x = Vector3{ m.m[0][0],m.m[0][1], m.m[0][2] };
	Vector3 y = Vector3{ m.m[1][0],m.m[1][1], m.m[1][2] };
	Vector3 z = Vector3{ m.m[2][0],m.m[2][1], m.m[2][2] };

	DrawLine3D(m_pos, m_pos + x * 100, 0xff0000);
	DrawLine3D(m_pos, m_pos + y * 100, 0x00ff00);
	DrawLine3D(m_pos, m_pos + z * 100, 0x0000ff);

	Vector2 iA = Input::GetInstance().GetLeftInputAxis();
	Vector3 inputAxis = {-iA.x, 0, iA.y};

	// ���͂��J�����ɉ�]
	Vector3 inputV = m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Back());


#endif
}

void Player::OnCollision(std::shared_ptr<Actor> other)
{
	m_isContactLockOnActor = (other == m_lockOnActor.lock());

	switch (other->GetKind())
	{
	case ActorKind::kEnemyAttack:
		printf("�U���ɓ�������\n");
		break;
	default:
		break;
	}
}

void Player::CommitMove()
{
	// ���x���������Ď擾
	const Vector3 vel = m_collidable->UpdateRigid();
	m_pos += vel;
	m_collidable->SetPos(m_pos + kCapsuleEndPosOffset * 0.5f);

	// ���b�N�I���J�[�\���̋���
	if (!m_lockOnActor.expired())
	{
		m_lockOnCursorPos = ConvWorldPosToScreenPos(m_lockOnActor.lock()->GetPos());
	}

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

void Player::EnableSwordCol(const float attackPower)
{
	m_sword->ColEnable();
	m_sword->SetAttackPower(attackPower);
}

void Player::DisableSword()
{
	m_sword->Disable();
}

void Player::DisableSwordCol()
{
	m_sword->ColDisable();
}
