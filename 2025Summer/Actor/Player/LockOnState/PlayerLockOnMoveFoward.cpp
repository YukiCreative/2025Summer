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

	// 内積の1～-1の四等分
	constexpr float kMoveDirThreshold = 0.25f;

	// ドライブ発生のための後ろ入力猶予
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

	if (modelAxisDot < -kMoveDirThreshold) // 後
	{
		return std::make_shared<PlayerLockOnMoveBack>(m_player);
	}
	else if (modelAxisDot < kMoveDirThreshold && cross.y > 0) // 右
	{
		return std::make_shared<PlayerLockOnMoveRight>(m_player);
	}
	else if (modelAxisDot < kMoveDirThreshold && cross.y < 0) // 左
	{
		return std::make_shared<PlayerLockOnMoveLeft>(m_player);
	}


	/// 攻撃
	if (input.IsTrigger("Attack"))
	{
		// 一定フレームの間に後ろに入力していたらドライブに派生

		bool isInputBack = false;
		int  searchDepth = 0;
		const int  searchMax = std::min(static_cast<int>(p->m_inputList.size()), kBackInputRespite);

		for (auto it = p->m_inputList.rbegin(); ; ++it)
		{
			if (searchDepth >= searchMax) break;
			// 指定の回数遡って合致する要素があるか調べる
			isInputBack |= (*it == PlayerInputDir::kBack);
			++searchDepth;
		}

		if (isInputBack)
		{
			// ドライブ
			return std::make_shared<PlayerShockWaveSlash>(m_player);
		}
		else
		{
			// 突進攻撃
			return std::make_shared<PlayerChargeAttack>(m_player);
		}
	}

	// 状態を記録
	p->SetInputDir(PlayerInputDir::kFront);

	return shared_from_this();
}
