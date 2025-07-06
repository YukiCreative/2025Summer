#include "PlayerJumpSlash.h"
#include "Player.h"
#include "Collidable.h"
#include <DxLib.h>
#include "AnimationModel.h"
#include "Input.h"

namespace
{
	constexpr int kChargeFrame = 40;
	constexpr float kChargeForce = 5.0f;

	constexpr int kEnableAttackFrame = kChargeFrame;
	constexpr int kDisableAttackFrame = kChargeFrame + 10;
	constexpr int kStateWholeFrame = 168 + kChargeFrame;
	// 前進するタイミング
	constexpr int kTrackFrame = 5;
	// 前進する力
	const float kTrackForce = 30.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 80;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 100;

	constexpr float kAttackPower = 150.0f;

	const std::string kAnimName = "Armature|JumpAttack";
	constexpr bool kIsLoopAnim = false;
}

PlayerJumpSlash::PlayerJumpSlash(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
}

PlayerJumpSlash::~PlayerJumpSlash()
{
}

void PlayerJumpSlash::Init()
{
	m_animName                   = kAnimName;
	m_isLoopAnim                 = kIsLoopAnim;
	m_enableAttackColFrame       = kEnableAttackFrame;
	m_disableAttackColFrame      = kDisableAttackFrame;
	m_stateTotalFrame            = kStateWholeFrame;
	m_trackFrame                 = kTrackFrame;
	m_trackForce                 = kTrackForce;
	m_acceptNextAttackInputFrame = kAcceptAttackInputFrame;
	m_enableComboFrame           = kEnableComboFrame;
	m_attackPower                = kAttackPower;
}

void PlayerJumpSlash::OptionalProcess()
{
	// 敵の位置に移動
	// ほぼ突進攻撃と変わらない

	auto p = m_player.lock();

	if (m_frame == 10)
	{
		p->m_model->Stop();
	}

	// 一定時間前進
	// ロックオンしている物に向かう
	if (m_frame <= kChargeFrame)
	{
		// プレイヤーから敵へのXZ成分の単位ベクトル
		const Vector3 pToEXZN = (p->m_lockOnActor.lock()->GetPos().XZ() - p->GetPos().XZ()).GetNormalize();

		// 突進
		p->GetCollidable().AddVel(VTransformSR({ 0,0,kChargeForce }, MGetRotVec2(Vector3::Foward(), pToEXZN)));
	}

	if (m_frame == kChargeFrame)
	{
		p->m_model->Play();
	}

	// もし突進中にロックオン対象に接触したら突進を止めたい
	if (p->m_isContactLockOnActor || Input::GetInstance().IsTrigger("Attack"))
	{
		// 突進が終わる直前のフレームだということにする
		// すでにそのラインを超えていれば何もしない
		m_frame = max(kChargeFrame - 1, m_frame);
	}
}
