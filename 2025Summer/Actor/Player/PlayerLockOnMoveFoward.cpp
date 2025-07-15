#include "PlayerLockOnMoveFoward.h"
#include "PlayerLockOnMoveBack.h"
#include "PlayerLockOnMoveRight.h"
#include "PlayerLockOnMoveLeft.h"
#include "PlayerLockOnIdle.h"
#include "Input.h"
#include "AnimationModel.h"
#include "Player.h"
#include <DxLib.h>
#include "Camera.h"
#include "PlayerChargeAttack.h"
#include "PlayerShockWaveSlash.h"

namespace
{
	const std::string kAnimName = "Armature|WalkFoward";

	// ���ς�1�`-1�̎l����
	constexpr float kMoveDirThreshold = 0.25f;

	// �h���C�u�����̂��߂̌����͗P�\
	constexpr int kBackInputRespite = 8;
}

PlayerLockOnMoveFoward::PlayerLockOnMoveFoward(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerLockOnMoveFoward::~PlayerLockOnMoveFoward()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveFoward::Update()
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

	if (modelAxisDot < -kMoveDirThreshold) // ��
	{
		return std::make_shared<PlayerLockOnMoveBack>(m_player);
	}
	else if (modelAxisDot < kMoveDirThreshold && cross.y > 0) // �E
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	else if (modelAxisDot < kMoveDirThreshold && cross.y < 0) // ��
	{
		return std::make_shared<PlayerLockOnMoveLeft>(m_player);
	}


	/// �U��
	if (input.IsTrigger("Attack"))
	{
		// ���t���[���̊ԂɌ��ɓ��͂��Ă�����h���C�u�ɔh��

		bool isInputBack = false;
		int  searchDepth = 0;
		const int  searchMax = std::min(static_cast<int>(p->m_inputList.size()), kBackInputRespite);

		for (auto it = p->m_inputList.rbegin(); ; ++it)
		{
			if (searchDepth >= searchMax) break;
			// �w��̉񐔑k���č��v����v�f�����邩���ׂ�
			isInputBack |= (*it == PlayerInputDir::kBack);
			++searchDepth;
		}

		if (isInputBack)
		{
			// �h���C�u
			return std::make_shared<PlayerShockWaveSlash>(m_player);
		}
		else
		{
			// �ːi�U��
			return std::make_shared<PlayerChargeAttack>(m_player);
		}
	}

	// ��Ԃ��L�^
	p->SetInputDir(PlayerInputDir::kFront);

	return shared_from_this();
}
