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

namespace
{
	const std::string kAnimName = "Armature|WalkRight";

	// 内積の1～-1の四等分
	constexpr float kMoveDirThreshold = 0.5f;
}

PlayerLockOnMoveRight::PlayerLockOnMoveRight(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// アニメーション
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerLockOnMoveRight::~PlayerLockOnMoveRight()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveRight::Update()
{
	// 移動
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	p->MoveWithoutRotate(kLockOnWalkSpeed);
	p->LockOnRotate();

	Vector3 inputAxis = Vector3{ input.GetLeftInputAxis().x, 0, input.GetLeftInputAxis().y };
	inputAxis.z *= -1;
	Vector3 cameraRotatedAxis = p->m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Foward());

	// 入力がなくなったらIdleへ
	if (cameraRotatedAxis.SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}

	if (input.IsTrigger("Attack"))
	{
		return std::make_shared<PlayerSlashDown>(m_player);
	}

	// 状態を記録
	p->SetInputDir(PlayerInputDir::kRight);

	const Vector3 modelDir = p->m_model->GetDirection();

	const Vector3 cameraRotatedAxisN = cameraRotatedAxis.GetNormalize();

	// キャラの向きに対して入力がどんな位置関係か調べたい
	const Vector3 cross = modelDir.Cross(cameraRotatedAxisN);

	const float modelAxisDot = modelDir.Dot(cameraRotatedAxisN);

	if (modelAxisDot > kMoveDirThreshold) // 前
	{
		return std::make_shared<PlayerLockOnMoveFoward>(m_player);
	}
	else if (modelAxisDot < -kMoveDirThreshold) // 後
	{
		return std::make_shared<PlayerLockOnMoveBack>(m_player);
	}
	else if (cross.y > 0) // 右
	{
		return shared_from_this();
	}
	else // 左
	{
		return std::make_shared<PlayerLockOnMoveLeft>(m_player);
	}
}
