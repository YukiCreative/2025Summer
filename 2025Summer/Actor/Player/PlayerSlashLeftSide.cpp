#include "PlayerSlashLeftSide.h"

#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Input.h"
#include "PlayerSlashUp.h"

namespace
{
	constexpr int kEnableAttackFrame = 10;
	constexpr int kDisableAttackFrame = 16;
	constexpr int kStateWholeFrame = 78;
	// 前進するタイミング
	constexpr int kForwardFrame = 12;
	// 前進する力
	const Vector3 kForwardVel = { 0, 0, -20.0f };
	// 次の攻撃入力の受付開始時間
	constexpr int kAcceptAttackInputFrame = 10;
	// 攻撃が派生するタイミング
	constexpr int kEnableComboFrame = 26;

	constexpr float kAttackPower = 100.0f;

	const std::string kAnimName = "Armature|LeftHorizontalSlash";
}

PlayerSlashLeftSide::PlayerSlashLeftSide(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0),
	m_isEnterAttack(false)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);
}

PlayerSlashLeftSide::~PlayerSlashLeftSide()
{
}

std::shared_ptr<PlayerState> PlayerSlashLeftSide::Update()
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
		// 入力があったら、その方向に動く
		// そうでなければ近くの敵の方向
		// さらに一定範囲内に敵もいなかったら現在のモデルの向きに前進

				// 入力があったら、その方向に動く
		const auto& inputAxis = input.GetLeftInputAxis();

		Vector3 vel;

		if (inputAxis.SqrMagnitude() > kMoveThreshold)
		{
			vel = VTransformSR(kForwardVel, MGetRotVec2(kForwardVel, { inputAxis.x, 0, inputAxis.y, }));
		}

		p->GetCollidable().AddVel(VTransformSR(kForwardVel, p->GetModelMatrix()));
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
