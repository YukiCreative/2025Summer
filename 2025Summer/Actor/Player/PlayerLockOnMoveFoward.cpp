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

namespace
{
	const std::string kAnimName = "Armature|WalkFoward";

	// “àÏ‚Ì1`-1‚Ìl“™•ª
	constexpr float kMoveDirThreshold = 0.5f;
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
	// ˆÚ“®
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	p->MoveWithoutRotate(kLockOnWalkSpeed);

	Vector3 inputAxis = Vector3{ input.GetLeftInputAxis().x, 0, input.GetLeftInputAxis().y };
	inputAxis.z *= -1;
	Vector3 cameraRotatedAxis = p->m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Foward());

	// “ü—Í‚ª‚È‚­‚È‚Á‚½‚çIdle‚Ö
	if (cameraRotatedAxis.SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}

	const Vector3 modelDir = p->m_model->GetDirection();

	const Vector3 cameraRotatedAxisN = cameraRotatedAxis.GetNormalize();

	// ƒLƒƒƒ‰‚ÌŒü‚«‚É‘Î‚µ‚Ä“ü—Í‚ª‚Ç‚ñ‚ÈˆÊ’uŠÖŒW‚©’²‚×‚½‚¢
	const Vector3 cross = modelDir.Cross(cameraRotatedAxisN);

	const float modelAxisDot = modelDir.Dot(cameraRotatedAxisN);

	// ‰E
	if (modelAxisDot > 0 - kMoveDirThreshold && modelAxisDot < 0 + kMoveDirThreshold
		&& cross.y > 0)
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	// ¶
	if (modelAxisDot > 0 - kMoveDirThreshold && modelAxisDot < 0 + kMoveDirThreshold
		&& cross.y < 0)
	{
		return std::make_shared<PlayerLockOnMoveLeft>(m_player);
	}
	// Œã
	if (modelAxisDot < -1 + kMoveDirThreshold)
	{
		return std::make_shared<PlayerLockOnMoveBack>(m_player);
	}

	/// UŒ‚
	if (input.IsTrigger("Attack"))
	{
		// “ËiUŒ‚
		return std::make_shared<PlayerChargeAttack>(m_player);
	}

	return shared_from_this();
}
