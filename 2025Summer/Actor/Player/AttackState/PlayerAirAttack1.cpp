#include "PlayerAirAttack1.h"
#include "../Player.h"
#include "PlayerAirAttack2.h"

#include "../../../Model/AnimationModel.h"

namespace
{
	const std::string kAnimName = "Armature|AirCombo1";
	constexpr bool kIsLoopAnim = false;

	constexpr int kEnableAttackFrame = 10;
	constexpr int kDisableAttackFrame = 25;
	constexpr int kStateWholeFrame = 58;
	// 前進するタイミング
	constexpr int kTrackFrame = 0;
	// 前進する力
	const float kTrackForce = 0.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 10;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 26;

	constexpr float kAttackPower = 100.0f;
	const Vector3 kKnockbackPower =  {0.0f, 10.0f, -2.0f};
	constexpr float kStunPower = 3.0f;

	const Vector3 kHoverForce = { 0.0f, 10.0f, 0.0f };
}

PlayerAirAttack1::PlayerAirAttack1(std::weak_ptr<Player> player) :
	PlayerAttackState(player)
{
	Init();
	PlayAnim();
	//m_player.lock()->m_model->SetAnimPlaySpeed(60);
}

PlayerAirAttack1::~PlayerAirAttack1()
{
}

void PlayerAirAttack1::Init()
{
	// 一段目なので剣を出す
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
	m_comboAttack = std::make_shared<PlayerAirAttack2>(m_player);
	m_stunPower = kStunPower;
}

void PlayerAirAttack1::OptionalProcess()
{
	if (m_frame == kEnableAttackFrame)
	{
		// 落下速度をリセット
		m_player.lock()->StopY();
		m_player.lock()->AddVel(kHoverForce);
	}
}
