#include "AnimationModel.h"
#include "Camera.h"
#include "Collidable.h"
#include "Geometry.h"
#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerLockOnIdle.h"
#include "PlayerLockOnMoveBack.h"
#include "PlayerLockOnMoveFoward.h"
#include "PlayerLockOnMoveLeft.h"
#include "PlayerLockOnMoveRight.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|Idle";

	// �}���x�ňړ����邩
	// 45�x��
	constexpr float kMoveDirThreshold = DX_PI_F * 0.25f;
}

PlayerLockOnIdle::PlayerLockOnIdle(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerLockOnIdle::~PlayerLockOnIdle()
{
}

std::shared_ptr<PlayerState> PlayerLockOnIdle::Update()
{
	auto p = m_player.lock();

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

	// �L�����N�^�[�̌����ƈړ������ɂ���đJ�ڂ����Ԃ��ς��

	DrawLine3D(p->GetPos(), p->GetPos() + cameraRotatedAxis * 100, 0xffff00);

	const Vector3 modelDir = -p->m_model->GetDirection();

	const Vector3 cameraRotatedAxisN = cameraRotatedAxis.GetNormalize();

	// �L�����̌����ɑ΂��ē��͂��ǂ�Ȉʒu�֌W�����ׂ���
	const Vector3 cross = modelDir.Cross(cameraRotatedAxisN);
	
	const float modelAxisDot = modelDir.Dot(cameraRotatedAxisN);

	// ���������͂������v���Ȃ�p�x���A�łȂ���Ε��@�ƒ�`����

	// �O�㍶�E�̕������[�V����

	// �E
	if (modelAxisDot > 0 - kMoveDirThreshold && modelAxisDot < 0 + kMoveDirThreshold
		&& cross.y > 0)
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	// ��
	if (modelAxisDot > 0 - kMoveDirThreshold && modelAxisDot < 0 + kMoveDirThreshold
		&& cross.y < 0)
	{
		return std::make_shared<PlayerLockOnMoveLeft>(m_player);
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
