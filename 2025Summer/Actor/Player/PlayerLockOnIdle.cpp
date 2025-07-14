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

	// ±何度で移動するか
	// 45度分
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

	printf("カメラに対しての入力X:%f,Y:%f\n", cameraRotatedAxis.x, cameraRotatedAxis.y);

	// キャラクターの向きと移動方向によって遷移する状態が変わる

	DrawLine3D(p->GetPos(), p->GetPos() + cameraRotatedAxis * 100, 0xffff00);

	const Vector3 modelDir = p->m_model->GetDirection();

	const Vector3 cameraRotatedAxisN = cameraRotatedAxis.GetNormalize();

	// キャラの向きに対して入力がどんな位置関係か調べたい
	const Vector3 cross = modelDir.Cross(cameraRotatedAxisN);
	
	const float modelAxisDot = modelDir.Dot(cameraRotatedAxisN);

	// 向き→入力が反時計回りなら角度正、でなければ負　と定義する

	// 前後左右の歩きモーション

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
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	else if (cross.y < 0) // 左
	{
		return std::make_shared<PlayerLockOnMoveLeft>(m_player);
	}

	// 攻撃
	if (input.IsTrigger("Attack"))
	{
		return std::make_shared<PlayerSlashDown>(m_player);
	}


	return shared_from_this();
}
