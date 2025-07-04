#include "PlayerSlashSide.h"
#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Input.h"
#include "PlayerSlashUp.h"
#include "PlayerSlashLeftSide.h"
#include "Rigid.h"

namespace
{
	constexpr int kEnableAttackFrame = 10;
	constexpr int kDisableAttackFrame = 25;
	constexpr int kStateWholeFrame = 58;
	// 前進するタイミング
	constexpr int kTrackFrame = 10;
	// 前進する力
	const float kTrackForce = 20.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 10;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 26;

	constexpr float kAttackPower = 100.0f;

	const std::string kAnimName = "Armature|SlashSide";
	constexpr bool kIsLoopAnim = false;
}

PlayerSlashSide::PlayerSlashSide(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
}

PlayerSlashSide::~PlayerSlashSide()
{
}

void PlayerSlashSide::Init()
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
	m_comboAttack = std::make_shared<PlayerSlashLeftSide>(m_player);
}