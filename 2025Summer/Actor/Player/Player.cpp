#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"
#include "Collidable.h"
#include "Rigid.h"
#include "CapsuleCollider.h"
#include "Camera.h"
#include <DxLib.h>
#include "PlayerState.h"
#include "PlayerIdle.h"
#include "ActorController.h"
#include "Game.h"
#include "Image.h"

namespace
{
	const PhysicalMaterial kPhysiMat =
	{
		0.1f, 0.05f, 0.1f
	};

	constexpr int kWeight = 10;

	constexpr float kCameraHSpeed = 0.0001f;
	constexpr float kCameraVSpeed = 0.0001f;

	const Vector3 kCapsuleEndPosOffset = {0, 200, 0};
	constexpr float kCapsuleRadius = 30;
	constexpr float kAnimPlaySpeed = 30.0f;

	const std::string kLockOnCursorFile = "LockOnCursor.png";

	const Vector3 kLockOnCameraPosOffsetLeft =  {-200, 200, -50};
	const Vector3 kLockOnCameraPosOffsetRight = { 200, 200, -50};
}

Player::Player() :
	m_targetPos(),
	Actor(false)
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

	m_state = std::make_shared<PlayerIdle>(weak_from_this());
}

void Player::Update()
{
	// ��Ԃ��X�V
	// �؂�ւ������Ⴄ�C���X�^���X�������Ă���
	m_state = m_state->Update();

	// ���b�N�I���{�^������������߂��̓G�����b�N�I��
	if (Input::GetInstance().IsTrigger("LockOn"))
	{
		LockOn();
	}
	
	// �ʏ�̃J������]
	CameraMove();
	
	// ���b�N�I�������G����ʓ��Ɏ��܂�悤�ɂ���
	if (!m_lockOnActor.expired())
	{
		LockOnRotate();
	}

	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	m_model->Update();

	m_camera.lock()->SetTargetPos(m_targetPos);
	m_model->SetPos(m_pos);
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

void Player::LockOn()
{
	// �������b�N�I�����Ȃ�A
	if (!m_lockOnActor.expired())
	{
		// �������ďI���
		m_lockOnActor.reset();
		return;
	}

	// ��ʒ����Ɉ�ԋ߂��A���b�N�I���ł���G��T��
	auto lockOnList = m_cont.lock()->SearchCanLockOnActor();

	// ��ʓ��ɂ��邩
	ActorList_t inScreenActor;
	for (auto& actor : lockOnList)
	{
		if (!CheckCameraViewClip(actor->GetPos()))
		{
			inScreenActor.emplace_back(actor);
		}
	}

	// �����Ȃ�������A�I���
	if (inScreenActor.empty()) return;

	// �Ƃ肠�����擪�̗v�f�����Ă���
	std::weak_ptr<Actor> centerActor = inScreenActor.front();
	const Vector3 kScreenCenterPos = {Game::kScreenHalfWidth, Game::kScreenHalfHeight, 0 };
	// ���S����̋���(�̓��)
	float centerPosLength = (kScreenCenterPos - ConvWorldPosToScreenPos(centerActor.lock()->GetPos())).SqrMagnitude();

	for (auto& actor : inScreenActor)
	{
		auto screenPos = ConvWorldPosToScreenPos(actor->GetPos());

		// ������̕����߂����
		float actorToCenterLength = (kScreenCenterPos - ConvWorldPosToScreenPos(actor->GetPos())).SqrMagnitude();
		if (actorToCenterLength > centerPosLength) continue;

		// �X�V
		centerActor = actor;
		centerPosLength = actorToCenterLength;
	}

	// ��܂���Actor�Ƀ��b�N�I��
	m_lockOnActor = centerActor;
}

void Player::LockOnRotate()
{
	// �v���C���[�ƃ��b�N�I���Ώۂ���ʊO�ɏo�Ȃ��悤�ɂ�����

	auto cam = m_camera.lock();

	// �J�����̈ʒu�����߂Ă��܂�

	// �v���C���[����ʉE����������
	if (ConvWorldPosToScreenPos(m_pos).x < Game::kScreenHalfWidth)
	{
		// ����
		cam->SetPos(m_pos + VTransformSR(kLockOnCameraPosOffsetLeft, m_model->GetMatrix()));
	}
	else
	{
		// ����
		cam->SetPos(m_pos + VTransformSR(kLockOnCameraPosOffsetRight, m_model->GetMatrix()));
	}
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
	const Vector3 dir = m_model->GetDirection() * -1; // �t�Ȃ̂Ŕ��]

	// ���x�ƌ����Ƃ̊O�ς��Ƃ��āA�o���l�ŉ�]
	Vector3 cross = dir.Cross(vel);
	const float dot = dir.Dot(vel);

	// �^���΂Ɉړ����悤�Ƃ���ƁA�������o�O��̂œ��ʂɏ���
	if (cross.y <= 0.01f && dot <= -0.9999f)
	{
		// �����𖳗�����]�����ăo�O��j�~���Ƃ�
		// �ォ��ł��}�����ԁA����Ă�����ł���H
		cross.y += 0.1f;
	}

	m_model->RotateUpVecY(cross.y * 0.3f);

	const float clampDot = max(dot, -0.1f);

	// �����̌����ƈړ��������������Ă���ƁA�ړ��ʂ����]����
	// �U������ۂɈ�u���Ε����Ɉړ�����݂�����
	m_collidable->AddVel(vel * clampDot);
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

#endif
}

void Player::OnCollision(std::shared_ptr<Actor> other)
{
	printf("�Ȃ񂩓������Ă�");
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
}
