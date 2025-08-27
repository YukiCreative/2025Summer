#include "PlayerAirAttack3.h"
#include "PlayerAirAttack1.h"
#include "../Player.h"
#include "../../../Physics/Rigid.h"
#include "../PlayerLanding.h"

namespace
{
	const std::string kStunAnimName = "Armature|AirCombo3";
	constexpr bool kIsLoopAnim = false;

	constexpr int kEnableAttackFrame = 9;
	constexpr int kDisableAttackFrame = 20;
	constexpr int kStateWholeFrame = 64;
	// 前進するタイミング
	constexpr int kTrackFrame = 0;
	// 前進する力
	const float kTrackForce = 0.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 20;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 40;

	constexpr float kAttackPower = 100.0f;
	const Vector3 kKnockbackPower = { 0.0f, -50.0f, 100.0f };
	constexpr float kStunPower = 4.0f;

	const Vector3 kHoverForce = { 0.0f, 10.0f, 0.0f };
}

PlayerAirAttack3::PlayerAirAttack3(std::weak_ptr<Player> player) :
	PlayerAttackState(player)
{
	m_beforeVel = m_player.lock()->GetRigid().GetVel();
	// 上昇している分は保存しない
	if (m_beforeVel.y > 0.0f)
	{
		m_beforeVel.y = 0.0f;
		m_player.lock()->StopY();
		m_player.lock()->AddVel(kHoverForce);
	}
}

PlayerAirAttack3::~PlayerAirAttack3()
{
	m_player.lock()->AddVel(m_beforeVel);
}

void PlayerAirAttack3::Init()
{
	m_actionKind = IncreaseStylishPointKind::kSlashDown;
	m_animName = kStunAnimName;
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
	m_comboAttack = std::make_shared<PlayerAirAttack1>(m_player);
	m_stunPower = kStunPower;
}

std::shared_ptr<PlayerState> PlayerAirAttack3::OptionalProcess()
{
	if (m_frame == kEnableAttackFrame)
	{
		// 落下速度をリセット
		m_player.lock()->StopY();
		m_player.lock()->AddVel(kHoverForce);
	}

	// 地面についてたら着地
	if (m_player.lock()->IsGround())
	{
		return std::make_shared<PlayerLanding>(m_player);
	}

	return shared_from_this();
}
