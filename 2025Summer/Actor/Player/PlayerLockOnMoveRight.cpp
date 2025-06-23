#include "PlayerLockOnMoveRight.h"
#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Input.h"
#include <DxLib.h>
#include "PlayerLockOnIdle.h"
#include "Camera.h"

namespace
{
	const std::string kAnimName = "Armature|WalkRight";
}

PlayerLockOnMoveRight::PlayerLockOnMoveRight(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// �A�j���[�V����
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerLockOnMoveRight::~PlayerLockOnMoveRight()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveRight::Update()
{
	// �ړ�
	auto p = m_player.lock();

	p->Move(kLockOnWalkSpeed);

	if (p->m_lockOnActor.expired())
	{
		return std::make_shared<PlayerIdle>(m_player);
	}

	// �v���C���[��G�����ɉ�]
	auto lockOnPosXZ = p->m_lockOnActor.lock()->GetPos().XZ();
	auto posXZ = p->GetPos().XZ();

	auto lockOnToPlayerXZ = (posXZ - lockOnPosXZ).GetNormalize();

	auto playerDir = p->m_model->GetDirection();

	auto dot = lockOnToPlayerXZ.Dot(playerDir);

	float rot = playerDir.Cross(lockOnToPlayerXZ).y * 0.2f;

	// ���傤�ǐ^���΂Ɍ����Ă����ꍇ�̏���
	if (dot < -0.9999f && rot < 0.0001f)
	{
		rot += 0.1f;
	}

	p->m_model->RotateUpVecY(rot);

	Vector3 inputAxis = Vector3{ Input::GetInstance().GetLeftInputAxis().x, 0, Input::GetInstance().GetLeftInputAxis().y };
	inputAxis.z *= -1;
	Vector3 cameraRotatedAxis = p->m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Foward());

	// ���͂��Ȃ��Ȃ�����Idle��
	if (cameraRotatedAxis.SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}

	return shared_from_this();
}
