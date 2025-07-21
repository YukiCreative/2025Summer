#include "PlayerShockWaveSlash.h"
#include "Player.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Geometry.h"

namespace
{
	constexpr int kEnableAttackFrame = 51;
	constexpr int kDisableAttackFrame = 57;

	// この攻撃は二回攻撃判定がある
	constexpr int kEnableAttackFrame2 = 69;
	constexpr int kDisableAttackFrame2 = 76;

	// 衝撃波タイミング
	constexpr int kShockWaveFrame1 = 53;
	constexpr int kShockWaveFrame2 = 71; 

	constexpr int kStateWholeFrame = 132;
	// 前進するタイミング
	constexpr int kTrackFrame = 51;
	constexpr int kTrackFrame2 = 69;
	// 前進する力
	const float kTrackForce = 20.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 80;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 120;

	constexpr float kAttackPower = 50.0f;
	constexpr float kAttackPower2 = 60.0f;

	const std::string kAnimName = "Armature|Drive";
	constexpr bool kIsLoopAnim = false;

	constexpr float kShockWaveRot1 = 80.0f;
	constexpr float kShockWaveRot2 = -80.0f;
	constexpr float kShockWaveAtk1 = 130.0f;
	constexpr float kShockWaveAtk2 = 150.0f;
	constexpr float kKnockbackPower1 = 10.0f;
	constexpr float kKnockbackPower2 = 30.0f;
}

PlayerShockWaveSlash::PlayerShockWaveSlash(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
	// 一段目なので自分で初期化する
	Init();
	PlayAnim();
}

PlayerShockWaveSlash::~PlayerShockWaveSlash()
{
}

void PlayerShockWaveSlash::Init()
{
	// 剣出す
	auto p = m_player.lock();
	p->EnableSword();

	// この代入操作は共通化できないだろうか
	// 外部データ化して取得すれば解決する
	m_animName = kAnimName;
	m_isLoopAnim = kIsLoopAnim;
	m_enableAttackColFrame = kEnableAttackFrame;
	m_disableAttackColFrame = kDisableAttackFrame;
	m_stateTotalFrame = kStateWholeFrame;
	m_trackFrame = kTrackFrame;
	m_trackForce = kTrackForce;
	m_enemyTrackForce = kTrackForce;
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

	// 二回目の前進
	if (m_frame == kTrackFrame2)
	{
		p->GetCollidable().AddVel(TrackingVec(kTrackForce, kTrackForce));
	}

	// 特定のフレームで、衝撃波を放つ
	if (m_frame == kShockWaveFrame1)
	{
		auto rot = MMult(MGetRotZ(kShockWaveRot1 * Geometry::kDegToRad), MGetRotElem(p->GetModelMatrix()));
		p->SpawnShockWave(rot, p->GetRightInexPos(), kShockWaveAtk1, kKnockbackPower1);
	}
	if (m_frame == kShockWaveFrame2)
	{
		auto rot = MMult(MGetRotZ(kShockWaveRot2 * Geometry::kDegToRad), MGetRotElem(p->GetModelMatrix()));
		p->SpawnShockWave(rot, p->GetRightInexPos(), kShockWaveAtk2, kKnockbackPower2);
	}
}
