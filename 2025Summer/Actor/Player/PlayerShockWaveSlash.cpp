#include "PlayerShockWaveSlash.h"
#include "Player.h"

namespace
{
	constexpr int kEnableAttackFrame = 54;
	constexpr int kDisableAttackFrame = 62;

	// この攻撃は二回攻撃判定がある
	constexpr int kEnableAttackFrame2 = 84;
	constexpr int kDisableAttackFrame2 = 92;

	// 衝撃波タイミング
	constexpr int kShockWaveFrame1 = 58;
	constexpr int kShockWaveFrame2 = 88;

	constexpr int kStateWholeFrame = 140;
	// 前進するタイミング
	constexpr int kTrackFrame = 20;
	// 前進する力
	const float kTrackForce = 20.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 80;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 120;

	constexpr float kAttackPower = 50.0f;
	constexpr float kAttackPower2 = 60.0f;

	constexpr float kShockWavePower = 100.0f;
	constexpr float kShockWavePower2 = 120.0f;

	const std::string kAnimName = "Armature|Drive";
	constexpr bool kIsLoopAnim = false;
}

PlayerShockWaveSlash::PlayerShockWaveSlash(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
}

PlayerShockWaveSlash::~PlayerShockWaveSlash()
{
}

void PlayerShockWaveSlash::Init()
{
	// この代入操作は共通化できないだろうか
	// 外部データ化して取得すれば解決する
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
}

void PlayerShockWaveSlash::OptionalProcess()
{
	auto p = m_player.lock();

	// 二回目の攻撃判定
	if (m_frame == kEnableAttackFrame2)
	{
		p->EnableSwordCol(kAttackPower2);
	}
	if (m_frame == kDisableAttackFrame2)
	{
		p->DisableSwordCol();
	}

	// 特定のフレームで、衝撃波を放つ
	if (m_frame == kShockWaveFrame1)
	{
		
	}
	if (m_frame == kShockWaveFrame2)
	{

	}
}
