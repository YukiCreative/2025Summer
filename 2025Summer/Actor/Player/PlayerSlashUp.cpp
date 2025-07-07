#include "PlayerSlashUp.h"
#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Input.h"
#include "Rigid.h"
#include "PlayerSlashDown.h"

namespace	
{
	constexpr int kEnableAttackFrame = 9;
	constexpr int kDisableAttackFrame = 20;
	constexpr int kStateWholeFrame = 78;
	// 前進するタイミング
	constexpr int kTrackFrame = 12;
	// 前進する力
	const float kTrackForce = 30.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 30;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 50;

	constexpr float kAttackPower = 130.0f;

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
	m_animName = kAnimName;
	m_isLoopAnim = kIsLoopAnim;
	m_enableAttackColFrame = kEnableAttackFrame;
	m_disableAttackColFrame = kDisableAttackFrame;
	m_stateTotalFrame = kStateWholeFrame;
	m_trackFrame = kTrackFrame;
	m_trackForce = kTrackForce;
	m_acceptNextAttackInputFrame = kAcceptAttackInputFrame;
	m_enableComboFrame = kEnableComboFrame;
	m_attackPower = kAttackPower;
	m_comboAttack = std::make_shared<PlayerSlashDown>(m_player);
}
