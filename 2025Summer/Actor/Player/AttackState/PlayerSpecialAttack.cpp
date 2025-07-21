#include "PlayerSpecialAttack.h"

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
	constexpr int kAcceptAttackInputFrame = 30;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 50;

	constexpr float kAttackPower = 130.0f;
	constexpr float kKnockbackPower = 30.0f;

	const std::string kAnimName = "Armature|SlashUp";
	constexpr bool kIsLoopAnim = false;
}

PlayerSpecialAttack::PlayerSpecialAttack(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
}

PlayerSpecialAttack::~PlayerSpecialAttack()
{
}

void PlayerSpecialAttack::Init()
{
	// とりあえず特定の攻撃で剣に当たり判定を付けるのは賛成
}

void PlayerSpecialAttack::OptionalProcess()
{

}
