#include "PlayerLockOnMove.h"
#include "../Player.h"
#include "../../../General/Input.h"
#include "../../../Camera/Camera.h"
#include "PlayerLockOnIdle.h"
#include "../../../Model/AnimationModel.h"

#include "PlayerLockOnMoveBack.h"
#include "PlayerLockOnMoveFoward.h"
#include "PlayerLockOnMoveRight.h"
#include "PlayerLockOnMoveLeft.h"

namespace
{
	// 内積の1～-1の四等分
	constexpr float kMoveDirThreshold = 0.5f;
}

PlayerLockOnMove::PlayerLockOnMove(std::weak_ptr<Player> parent, const std::string& animName, const PlayerInputDir dir) :
	PlayerState(parent),
	m_dir(dir)
{
	m_player.lock()->ChangeAnim(animName, true);
}

PlayerLockOnMove::~PlayerLockOnMove()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMove::Update()
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

	// 状態を記録
	p->SetInputDir(m_dir);

	// 攻撃モーション
	if (input.IsTrigger("Attack"))
	{
		return OnAttack();
	}

	if (modelAxisDot > kMoveDirThreshold) // 前
	{
		if (m_dir == PlayerInputDir::kFront) return shared_from_this();

		return std::make_shared<PlayerLockOnMoveFoward>(m_player);
	}
	else if (modelAxisDot < -kMoveDirThreshold) // 後
	{
		if (m_dir == PlayerInputDir::kBack) return shared_from_this();

		return std::make_shared<PlayerLockOnMoveBack>(m_player);
	}
	else if (cross.y > 0) // 右
	{
		if (m_dir == PlayerInputDir::kRight) return shared_from_this();

		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	else // 左
	{
		if (m_dir == PlayerInputDir::kLeft) return shared_from_this();

		return std::make_shared<PlayerLockOnMoveLeft>(m_player);
	}

	return shared_from_this();
}
