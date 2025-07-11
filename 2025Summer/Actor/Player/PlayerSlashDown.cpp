#include "PlayerSlashDown.h"
#include "PlayerSlashLeftSide.h"
#include "Player.h"

namespace
{
	const std::string kAnimName = "Armature|SlashDown";
	constexpr bool kIsLoopAnim = false;

	constexpr int kEnableAttackFrame = 10;
	constexpr int kDisableAttackFrame = 25;
	constexpr int kStateWholeFrame = 58;
	// 前進するタイミング
	constexpr int kTrackFrame = 0;
	// 前進する力
	const float kTrackForce = 20.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 10;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 26;

	constexpr float kAttackPower = 100.0f;
	constexpr float kKnockbackPower = 10.0f;
}

PlayerSlashDown::PlayerSlashDown(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
	// 一段目なので自分で初期化する
	Init();
	PlayAnim();
}

PlayerSlashDown::~PlayerSlashDown()
{
}


void PlayerSlashDown::Init()
{
	// 一段目なので剣を出す
	m_player.lock()->EnableSword();

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
	m_comboAttack = std::make_shared<PlayerSlashLeftSide>(m_player);
}