#include "PlayerKnockUpSlash.h"
#include "../Player.h"
#include "../../../General/Input.h"

namespace
{
	constexpr int kEnableAttackFrame = 9;
	constexpr int kDisableAttackFrame = 20;
	constexpr int kStateWholeFrame = 60;
	// 前進するタイミング
	constexpr int kTrackFrame = 12;
	// 前進する力
	const float kTrackForce = 10.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 20;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 30;

	constexpr float kAttackPower = 130.0f;
	const Vector3 kKnockbackPower = { 0.0f, 40.0f, 1.0f };

	const std::string kAnimName = "Armature|SlashUp";
	constexpr bool kIsLoopAnim = false;

	constexpr int kJumpFrame = 9;
	const Vector3 kJumpForce = {0.0f, 40.0f, 0.0f};
}

PlayerKnockUpSlash::PlayerKnockUpSlash(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent),
	m_isLongPress(true)
{
	Init();
	PlayAnim();
}

PlayerKnockUpSlash::~PlayerKnockUpSlash()
{
}

void PlayerKnockUpSlash::Init()
{
	m_player.lock()->EnableSword();

	m_actionKind = IncreaseStylishPointKind::kSlashDown;
	m_animName = kAnimName;
	m_isLoopAnim = kIsLoopAnim;
	m_enableAttackColFrame = kEnableAttackFrame;
	m_disableAttackColFrame = kDisableAttackFrame;
	m_stateTotalFrame = kStateWholeFrame;
	m_trackFrame = kTrackFrame;
	m_trackForce = kTrackForce;
	m_enemyTrackForce = kTrackForce * 3.0f;
	m_acceptNextAttackInputFrame = kAcceptAttackInputFrame;
	m_enableComboFrame = kEnableComboFrame;
	m_attackPower = kAttackPower;
	m_knockbackPower = kKnockbackPower;
}

void PlayerKnockUpSlash::OptionalProcess()
{
	// 一度離したらfalseになる
	m_isLongPress &= Input::GetInstance().IsPressed("Attack");

	// 特定のフレームまで攻撃を長押ししていたら飛び上がる
	if (m_frame == kJumpFrame && m_isLongPress)
	{
		m_player.lock()->AddVel(kJumpForce);
	}
}
