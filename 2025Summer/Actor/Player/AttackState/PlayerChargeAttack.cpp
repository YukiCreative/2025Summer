#include "PlayerChargeAttack.h"
#include "Player.h"
#include "Collidable.h"
#include <DxLib.h>
#include "AnimationModel.h"
#include "Input.h"

namespace
{
	// このフレームまで前進
	constexpr int kChargeFrame = 30;
	constexpr float kChargeForce = 5.0f;

	constexpr int kEnableAttackFrame = kChargeFrame;
	constexpr int kDisableAttackFrame = kChargeFrame + 10;
	constexpr int kStateWholeFrame = 60 + kChargeFrame;
	// 前進するタイミング
	constexpr int kTrackFrame = 5;
	// 前進する力
	const float kTrackForce = 30.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 30;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 50 + kChargeFrame;

	constexpr float kAttackPower = 2000.0f;
	constexpr float kKnockbackPower = 40.0f;

	const std::string kAnimName = "Armature|LeftHorizontalSlash";
	constexpr bool kIsLoopAnim = false;
}

PlayerChargeAttack::PlayerChargeAttack(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
	// 一段目なので自分で初期化する
	Init();
	PlayAnim();
}

PlayerChargeAttack::~PlayerChargeAttack()
{
}

void PlayerChargeAttack::Init()
{
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
	m_knockbackPower = kKnockbackPower;

	// 剣出す
	auto p = m_player.lock();
	p->EnableSword();
}

void PlayerChargeAttack::OptionalProcess()
{
	auto p = m_player.lock();

	if (m_frame == 10)
	{
		p->m_model->Stop();
	}

	// 一定時間前進
	// ロックオンしている物に向かう
	// ロックオンしてなければそのまま前進
	if (m_frame <= kChargeFrame)
	{
		if (p->IsLockOn())
		{
			// プレイヤーから敵へのXZ成分の単位ベクトル
			const Vector3 pToEXZN = (p->m_lockOnActor.lock()->GetPos().XZ() - p->GetPos().XZ()).GetNormalize();

			// 突進
			p->GetCollidable().AddVel(VTransformSR({ 0,0,kChargeForce }, MGetRotVec2(Vector3::Foward(), pToEXZN)));
		}
		else
		{
			// 前進
			p->GetCollidable().AddVel(VTransformSR({ 0,0,kChargeForce }, p->GetModelMatrix()));
		}
	}

	if (m_frame == kChargeFrame)
	{
		p->m_model->Play();
	}

	// 向きの変更は上位のステートですでに行っているのでやらない

	// もし突進中にロックオン対象に接触したら突進を止めたい
	if (p->m_isContactLockOnActor || Input::GetInstance().IsTrigger("Attack"))
	{
		// 突進が終わる直前のフレームだということにする
		// すでにそのラインを超えていれば何もしない
		m_frame = std::max(kChargeFrame-1, m_frame);
	}
}
