#include "PlayerSlashUp.h"
#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Input.h"
#include "Rigid.h"

namespace	
{
	constexpr int kEnableAttackFrame = 9;
	constexpr int kDisableAttackFrame = 20;
	constexpr int kStateWholeFrame = 78;
	// 前進するタイミング
	constexpr int kForwardFrame = 15;
	// 前進する力
	const float kForwardForce = 30.0f;
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 10;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 26;

	constexpr float kAttackPower = 130.0f;

	const std::string kAnimName = "Armature|SlashUp";
}

PlayerSlashUp::PlayerSlashUp(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0),
	m_isEnterAttack(false)
{
	// アニメーション
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);
}

PlayerSlashUp::~PlayerSlashUp()
{
}

std::shared_ptr<PlayerState> PlayerSlashUp::Update()
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

	// 待機状態へ遷移
	if (m_frame >= kStateWholeFrame)
	{
		p->DiaableSword();

		return std::make_shared<PlayerIdle>(m_player);
	}

	++m_frame;

	return shared_from_this();
}
