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

	// 内積の1～-1の四等分
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

	const Vector3 modelDir = p->m_model->GetDirection();

	const Vector3 cameraRotatedAxisN = cameraRotatedAxis.GetNormalize();

	// キャラの向きに対して入力がどんな位置関係か調べたい
	const Vector3 cross = modelDir.Cross(cameraRotatedAxisN);

	const float modelAxisDot = modelDir.Dot(cameraRotatedAxisN);

	// 攻撃モーション
	if (input.IsTrigger("Attack"))
	{
		// 通常のコンボ

		return std::make_shared<PlayerSlashDown>(m_player);
	}

	// 状態を記録
	p->SetInputDir(PlayerInputDir::kBack);

	// 前後左右の歩きモーション

	if (modelAxisDot > kMoveDirThreshold) // 前
	{
		return std::make_shared<PlayerLockOnMoveFoward>(m_player);
	}
	else if (modelAxisDot < -kMoveDirThreshold) // 後
	{
		return shared_from_this();
	}
	else if (cross.y > 0) // 右
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	else // 左
	{
		return std::make_shared<PlayerLockOnMoveLeft>(m_player);
	}
}
