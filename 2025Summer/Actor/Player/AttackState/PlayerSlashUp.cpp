#include "PlayerSlashUp.h"
#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Input.h"
#include "Rigid.h"
#include "PlayerFinishAttack.h"

namespace	
{
	constexpr int kEnableAttackFrame = 9;
	constexpr int kDisableAttackFrame = 20;
	constexpr int kStateWholeFrame = 78;
	// 前進するタイミング
	constexpr int kTrackFrame = 12;
	// 前進する力
	const float kTrackForce = 10.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 20;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 40;

	constexpr float kAttackPower = 130.0f;
	constexpr float kKnockbackPower = 30.0f;

	const std::string kAnimName = "Armature|SlashUp";
	constexpr bool kIsLoopAnim = false;
}

PlayerSlashUp::PlayerSlashUp(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
}

PlayerSlashUp::~PlayerSlashUp()
{
}

void PlayerSlashUp::Init()
{
	m_actionKind = IncreaseStylishPointKind::kSlashUp;
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
	m_comboAttack = std::make_shared<PlayerFinishAttack>(m_player);
}
