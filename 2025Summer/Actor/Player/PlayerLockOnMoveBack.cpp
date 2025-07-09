#include "AnimationModel.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"
#include "PlayerLockOnIdle.h"
#include "PlayerLockOnMoveBack.h"
#include "PlayerLockOnMoveFoward.h"
#include "PlayerLockOnMoveLeft.h"
#include "PlayerLockOnMoveRight.h"
#include "PlayerSlashDown.h"
#include <DxLib.h>

namespace
{
	const std::string kAnimName = "Armature|WalkBack";

	// ���ς�1�`-1�̎l����
	constexpr float kMoveDirThreshold = 0.5f;
}

PlayerLockOnMoveBack::PlayerLockOnMoveBack(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerLockOnMoveBack::~PlayerLockOnMoveBack()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveBack::Update()
{
	// �ړ�
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	p->MoveWithoutRotate(kLockOnWalkSpeed);
	p->LockOnRotate();

	Vector3 inputAxis = Vector3{ input.GetLeftInputAxis().x, 0, input.GetLeftInputAxis().y };
	inputAxis.z *= -1;
	Vector3 cameraRotatedAxis = p->m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Foward());

	// ���͂��Ȃ��Ȃ�����Idle��
	if (cameraRotatedAxis.SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}

	const Vector3 modelDir = p->m_model->GetDirection();

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

	// �U�����[�V����
	if (input.IsTrigger("Attack"))
	{
		// �ʏ�̃R���{

		return std::make_shared<PlayerSlashDown>(m_player);
	}

	// ��Ԃ��L�^
	p->SetInputDir(PlayerInputDir::kBack);

	return shared_from_this();
}
