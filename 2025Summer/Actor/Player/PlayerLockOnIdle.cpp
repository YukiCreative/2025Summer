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
#include "PlayerSlashDown.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|Idle";

	// �}���x�ňړ����邩
	// 45�x��
	constexpr float kMoveDirThreshold = 0.5f;
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
	auto& input = Input::GetInstance();
		
	Vector3 inputAxis = Vector3{ input.GetLeftInputAxis().x, 0, input.GetLeftInputAxis().y };
	inputAxis.z *= -1;
	Vector3 cameraRotatedAxis = p->m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Foward());

	printf("�J�����ɑ΂��Ă̓���X:%f,Y:%f\n", cameraRotatedAxis.x, cameraRotatedAxis.y);

	// �L�����N�^�[�̌����ƈړ������ɂ���đJ�ڂ����Ԃ��ς��

	DrawLine3D(p->GetPos(), p->GetPos() + cameraRotatedAxis * 100, 0xffff00);

	const Vector3 modelDir = p->m_model->GetDirection();

	const Vector3 cameraRotatedAxisN = cameraRotatedAxis.GetNormalize();

	// �L�����̌����ɑ΂��ē��͂��ǂ�Ȉʒu�֌W�����ׂ���
	const Vector3 cross = modelDir.Cross(cameraRotatedAxisN);
	
	const float modelAxisDot = modelDir.Dot(cameraRotatedAxisN);

	// ���������͂������v���Ȃ�p�x���A�łȂ���Ε��@�ƒ�`����

	// �O�㍶�E�̕������[�V����

	if (modelAxisDot > kMoveDirThreshold) // �O
	{
		return std::make_shared<PlayerLockOnMoveFoward>(m_player);
	}
	else if (modelAxisDot < -kMoveDirThreshold) // ��
	{
		return std::make_shared<PlayerLockOnMoveBack>(m_player);
	}
	else if (cross.y > 0) // �E
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	else if (cross.y < 0) // ��
	{
		return std::make_shared<PlayerLockOnMoveLeft>(m_player);
	}

	// �U��
	if (input.IsTrigger("Attack"))
	{
		return std::make_shared<PlayerSlashDown>(m_player);
	}


	return shared_from_this();
}
