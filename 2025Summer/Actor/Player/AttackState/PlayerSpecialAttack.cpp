#include "PlayerSpecialAttack.h"
#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"
#include "PlayerMove.h"
#include "PlayerIdle.h"
#include "Camera.h"

namespace
{
	// 消えるフレーム
	constexpr int kDisappearFrame = 40;
	constexpr int kAttackFrame = 120;
	constexpr int kAppearFrame = 200;
	constexpr int kStateWholeFrame = 250;
	// 移動などに派生できるフレーム
	constexpr int kEnableTransitionFrame = 230;

	constexpr float kCameraDistance = 500.0f;

	const std::string kStartAnimName = "Armature|SpecialAttack";
	const std::string kEndAnimName = "Armature|FrontStop";
	constexpr bool kIsLoopAnim = false;
}

PlayerSpecialAttack::PlayerSpecialAttack(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
	auto p = m_player.lock();

	// アニメーション
	p->ChangeAnim(kStartAnimName, kIsLoopAnim);
	// 無敵
	p->SetInvincibility(true);

	p->SetCanLockOn(false);
	m_canCrossState = true;

	p->DisableSwordCol();

	p->m_camera.lock()->SetTargetDistance(kCameraDistance);
}

PlayerSpecialAttack::~PlayerSpecialAttack()
{
	if (m_player.expired()) return;

 	auto p =  m_player.lock();

	p->m_camera.lock()->SetCameraDistanceDefault();
	p->SetInvincibility(false);
	p->SetCanLockOn(true);
}

std::shared_ptr<PlayerState> PlayerSpecialAttack::Update()
{
	auto p = m_player.lock();

	// 特定のフレームで消える
	if (m_frame == kDisappearFrame)
	{
		p->Disappear();
	}

	// 特定のフレームで攻撃
	if (m_frame == kAttackFrame)
	{
		p->SpecialAttack();
	}

	// 攻撃が終わったら戻ってくるアニメーションを流す
	if (m_frame == kAppearFrame)
	{
		p->Apeear();
		p->ChangeAnim(kEndAnimName, kIsLoopAnim);
	}

	// 入力があればMove
	if (m_frame > kEnableTransitionFrame)
	{
		if (Input::GetInstance().GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
		{
			return std::make_shared<PlayerMove>(m_player);
		}
	}

	// アニメーションが終わればIdleに
	if (m_frame > kStateWholeFrame)
	{
		return std::make_shared<PlayerIdle>(m_player);
	}

	++m_frame;

	return shared_from_this();
}
