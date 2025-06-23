#include "PlayerLockOnIdle.h"
#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "PlayerLockOnMoveRight.h"
#include "Input.h"
#include <DxLib.h>
#include "Collidable.h"
#include "Geometry.h"
#include "Camera.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";

	// ±何度で移動するか
	constexpr float kMoveDirThreshold = 0.1f;
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

	// ロックオンが解除されていたら状態遷移
	if (p->m_lockOnActor.expired())
	{
		return std::make_shared<PlayerIdle>(m_player);
	}

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

	// キャラクターの向きと移動方向によって遷移する状態が変わる

	DrawLine3D(p->GetPos(), p->GetPos() + cameraRotatedAxis * 100, 0xffff00);

	const Vector3 modelDir = -p->m_model->GetDirection();

	// キャラの向きに対して入力がどんな位置関係か調べたい
	const Vector3 cross = modelDir.Cross(cameraRotatedAxis);
	
	const float modelAxisDot = modelDir.Dot(cameraRotatedAxis);

	// 向き→入力が反時計回りなら角度正、でなければ負　と定義する

	// 前後左右の歩きモーション

	// 右
	if (modelAxisDot > 0 - kMoveDirThreshold && modelAxisDot < 0 + kMoveDirThreshold
		&& cross.y > 0)
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	// 左
	if (modelAxisDot > 0 - kMoveDirThreshold && modelAxisDot < 0 + kMoveDirThreshold
		&& cross.y < 0)
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	// 前
	if (modelAxisDot > 1 - kMoveDirThreshold)
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	// 後
	if (modelAxisDot < -1 + kMoveDirThreshold)
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}

	return shared_from_this();
}
