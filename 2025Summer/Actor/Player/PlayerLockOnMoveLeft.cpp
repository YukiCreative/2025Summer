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
#include "PlayerSlashDown.h"

namespace
{
	const std::string kAnimName = "Armature|WalkLeft";

	// 内積の1～-1の四等分
	constexpr float kMoveDirThreshold = 0.5f;
}

// 重複コードが多いぜ
PlayerLockOnMoveLeft::PlayerLockOnMoveLeft(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// アニメーション
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerLockOnMoveLeft::~PlayerLockOnMoveLeft()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveLeft::Update()
{
	// 移動
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	p->MoveWithoutRotate(kLockOnWalkSpeed);

	Vector3 inputAxis = Vector3{ input.GetLeftInputAxis().x, 0, input.GetLeftInputAxis().y };
	inputAxis.z *= -1;
	Vector3 cameraRotatedAxis = p->m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Foward());

	// 入力がなくなったらIdleへ
	if (cameraRotatedAxis.SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}

	const Vector3 modelDir = p->m_model->GetDirection();

	const Vector3 cameraRotatedAxisN = cameraRotatedAxis.GetNormalize();

	// キャラの向きに対して入力がどんな位置関係か調べたい
	const Vector3 cross = modelDir.Cross(cameraRotatedAxisN);

	const float modelAxisDot = modelDir.Dot(cameraRotatedAxisN);

	// 右
	if (modelAxisDot > 0 - kMoveDirThreshold && modelAxisDot < 0 + kMoveDirThreshold
		&& cross.y > 0)
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	// 前
	if (modelAxisDot > 1 - kMoveDirThreshold)
	{
		return std::make_shared<PlayerLockOnMoveFoward>(m_player);
	}
	// 後
	if (modelAxisDot < -1 + kMoveDirThreshold)
	{
		return std::make_shared<PlayerLockOnMoveBack>(m_player);
	}

	// 攻撃モーション
	if (input.IsTrigger("Attack"))
	{
		// 通常のコンボ

		return std::make_shared<PlayerSlashDown>(m_player);
	}

	return shared_from_this();
}
