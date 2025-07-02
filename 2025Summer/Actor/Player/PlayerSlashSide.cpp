#include "PlayerSlashSide.h"
#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Input.h"
#include "PlayerSlashUp.h"
#include "Rigid.h"

namespace
{
	constexpr int kEnableAttackFrame = 10;
	constexpr int kDisableAttackFrame = 25;
	constexpr int kStateWholeFrame = 58;
	// 前進するタイミング
	constexpr int kForwardFrame = 10;
	// 前進する力
	const float kForwardForce = 20.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 10;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 26;

	constexpr float kAttackPower = 100.0f;

	const std::string kAnimName = "Armature|SlashSide";
}

PlayerSlashSide::PlayerSlashSide(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0),
	m_isEnterAttack(false)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);
}

PlayerSlashSide::~PlayerSlashSide()
{
}

std::shared_ptr<PlayerState> PlayerSlashSide::Update()
{
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	// 剣の攻撃判定を有効化
	if (m_frame == kEnableAttackFrame)
	{
		p->EnableSwordCol(kAttackPower);
	}
	if (m_frame == kDisableAttackFrame)
	{
		p->DisableSwordCol();
	}

	// 攻撃した瞬間移動
	if (m_frame == kForwardFrame)
	{
		p->GetRigid().AddVel(TrackingVec(kForwardForce));
	}

	// 先行して入力をとっておく
	if (m_frame >= kAcceptAttackInputFrame)
	{
		m_isEnterAttack |= input.IsTrigger("Attack");
	}

	// 次の攻撃へ
	if (m_frame >= kEnableComboFrame && m_isEnterAttack)
	{
		return std::make_shared<PlayerSlashUp>(m_player);
	}

	// 待機状態へ遷移
	if (m_frame >= kStateWholeFrame)
	{
		p->DiaableSword();

		return std::make_shared<PlayerIdle>(m_player);
	}

	++m_frame;

	return shared_from_this();
}
