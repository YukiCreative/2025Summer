#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"
#include "Collidable.h"
#include "Rigid.h"
#include "SphereCollider.h"
#include "Camera.h"
#include <DxLib.h>

namespace
{
	const PhysicalMaterial kPhysiMat =
	{
		0.1f, 0.05f, 0.1f
	};

	const Vector3 kCameraTargetOffset = { 0, 200, 0 };

	constexpr float kRunSpeed    = 0.001f;
	constexpr float kDashSpeed   = 0.002f;
	constexpr float kCameraHSpeed = 0.0001f;
	constexpr float kCameraVSpeed = 0.0001f;
}

Player::Player()
{
}

void Player::Init(const std::weak_ptr<Camera> camera)
{
	m_camera = camera;

	m_model = std::make_shared<AnimationModel>();
	m_model->Init("Data/Model/Player.mv1", 3);

	auto col = std::make_shared<SphereCollider>();
	col->Init(m_pos, 1, false, false, 1);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysiMat);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);
}

void Player::Update()
{
	Move();

	// �J�����𓮂���
	Vector2 rightAxis = Input::GetInstance().GetRightInputAxis();
	rightAxis.y *= -1;

	m_camera.lock()->RotateCameraUpVecY(rightAxis.x * kCameraHSpeed);
	m_camera.lock()->RotateCameraV(rightAxis.y * kCameraVSpeed);

	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	m_camera.lock()->SetTargetPos(m_pos + kCameraTargetOffset);
	m_model->SetPos(m_pos);
}

void Player::Move()
{
	// ���͂ňړ�
	Vector2 inputAxis = Input::GetInstance().GetLeftInputAxis();
	inputAxis.x *= -1;		// ���E(�������͏㉺)�����]���Ă���̂Œ���
	// �A�i���O���͂𐶂��������̂Ő��K���͂��Ȃ�

	Vector3 vel = Vector3{ inputAxis.x, 0,inputAxis.y } *kRunSpeed;
	// �ړ��ʂ��J�����̌����ɕ␳
	vel = m_camera.lock()->RotateVecToCameraDirXZ(vel, Vector3::Back());

	// �v���C���[�̌��݂̌������擾
	const Vector3 dir = m_model->GetDirection() * -1; // �t�Ȃ̂Ŕ��]

	// ���x�ƌ����Ƃ̊O�ς��Ƃ��āA�o���l�ŉ�]
	Vector3 cross = dir.Cross(vel);
	const float dot = dir.Dot(vel);

	// �^���΂Ɉړ����悤�Ƃ���ƁA�������o�O��̂œ��ʂɏ���
	if (cross.y <= 0.0001f && dot <= -0.9999f)
	{
		// �����𖳗�����]�����ăo�O��j�~���Ƃ�
		// �ォ��ł��}�����ԁA����Ă�����ł���H
		cross.y += 0.1f;
	}

	m_model->RotateUpVecY(cross.y * 0.3f);

	// �����̌����ƈړ��������������Ă���ƁA�ړ��ʂ����]����
	// �U������ۂɈ�u���Ε����Ɉړ�����݂�����
	m_collidable->AddVel(vel * dot);
}

void Player::Draw() const
{
	m_model->Draw();
}

void Player::CommitMove()
{
	// ���x���������Ď擾
	m_pos += m_collidable->UpdateRigid();
}
