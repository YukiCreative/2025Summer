#include "AnimationModel.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"
#include "PlayerLockOnIdle.h"
#include "PlayerLockOnMoveBack.h"
#include "PlayerLockOnMoveFoward.h"
#include "PlayerLockOnMoveLeft.h"
#include "PlayerLockOnMoveRight.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|WalkLeft";

	// ���ς�1�`-1�̎l����
	constexpr float kMoveDirThreshold = 0.5f;
}

// �d���R�[�h��������
PlayerLockOnMoveLeft::PlayerLockOnMoveLeft(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// �A�j���[�V����
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerLockOnMoveLeft::~PlayerLockOnMoveLeft()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveLeft::Update()
{
	// �ړ�
	auto p = m_player.lock();

	p->MoveWithoutRotate(kLockOnWalkSpeed);

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

	const Vector3 modelDir = -p->m_model->GetDirection();

	const Vector3 cameraRotatedAxisN = cameraRotatedAxis.GetNormalize();

	// �L�����̌����ɑ΂��ē��͂��ǂ�Ȉʒu�֌W�����ׂ���
	const Vector3 cross = modelDir.Cross(cameraRotatedAxisN);

	const float modelAxisDot = modelDir.Dot(cameraRotatedAxisN);

	// �E
	if (modelAxisDot > 0 - kMoveDirThreshold && modelAxisDot < 0 + kMoveDirThreshold
		&& cross.y > 0)
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	// �O
	if (modelAxisDot > 1 - kMoveDirThreshold)
	{
		return std::make_shared<PlayerLockOnMoveFoward>(m_player);
	}
	// ��
	if (modelAxisDot < -1 + kMoveDirThreshold)
	{
		return std::make_shared<PlayerLockOnMoveBack>(m_player);
	}

	return shared_from_this();
}
