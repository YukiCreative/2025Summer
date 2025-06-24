#include "PlayerLockOnMoveRight.h"
#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Input.h"
#include <DxLib.h>
#include "PlayerLockOnIdle.h"
#include "PlayerLockOnMoveLeft.h"
#include "PlayerLockOnMoveFoward.h"
#include "PlayerLockOnMoveBack.h"
#include "Camera.h"

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
	printf("RightMoveです");
}

PlayerLockOnMoveRight::~PlayerLockOnMoveRight()
{
}

std::shared_ptr<PlayerState> PlayerLockOnMoveRight::Update()
{
	// 移動
	auto p = m_player.lock();

	p->MoveWithoutRotate(kLockOnWalkSpeed);

	// プレイヤーを敵方向に回転
	auto lockOnPosXZ = p->m_lockOnActor.lock()->GetPos().XZ();
	auto posXZ = p->GetPos().XZ();

	auto lockOnToPlayerXZ = (posXZ - lockOnPosXZ).GetNormalize();

	auto playerDir = p->m_model->GetDirection();

	auto dot = lockOnToPlayerXZ.Dot(playerDir);

	float rot = playerDir.Cross(lockOnToPlayerXZ).y * 0.2f;

	// ちょうど真反対に向いていた場合の処理
	if (dot < -0.9999f && rot < 0.0001f)
	{
		rot += 0.1f;
	}

	p->m_model->RotateUpVecY(rot);

	Vector3 inputAxis = Vector3{ Input::GetInstance().GetLeftInputAxis().x, 0, Input::GetInstance().GetLeftInputAxis().y };
	inputAxis.z *= -1;
	Vector3 cameraRotatedAxis = p->m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Foward());

	// 入力がなくなったらIdleへ
	if (cameraRotatedAxis.SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}

	const Vector3 modelDir = -p->m_model->GetDirection();

	const Vector3 cameraRotatedAxisN = cameraRotatedAxis.GetNormalize();

	// キャラの向きに対して入力がどんな位置関係か調べたい
	const Vector3 cross = modelDir.Cross(cameraRotatedAxisN);

	const float modelAxisDot = modelDir.Dot(cameraRotatedAxisN);

	// 左
	if (modelAxisDot > 0 - kMoveDirThreshold && modelAxisDot < 0 + kMoveDirThreshold
		&& cross.y < 0)
	{
		return std::make_shared<PlayerLockOnMoveLeft>(m_player);
	}
	// 前
	if (modelAxisDot >= 1 - kMoveDirThreshold)
	{
		return std::make_shared<PlayerLockOnMoveFoward>(m_player);
	}
	// 後
	if (modelAxisDot <= -1 + kMoveDirThreshold)
	{
		return std::make_shared<PlayerLockOnMoveBack>(m_player);
	}

	return shared_from_this();
}
