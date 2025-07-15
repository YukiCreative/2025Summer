#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "AnimationModel.h"
#include "PlayerSlashDown.h"
#include "Collidable.h"
#include <DxLib.h>
#include "PlayerLockOnIdle.h"

namespace
{
	constexpr float kRunSpeed = 0.0018f;
	constexpr int   kDashFrame = 120;

	const std::string kMoveAnimName = "Armature|RunDash";

	// 移動切りの力
	constexpr float kSashAttackForce = 30.0f;
}

PlayerMove::PlayerMove(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_moveFrame(0)
{
	// ロックオン状態ならアニメーションを切り替えない
	if (!m_player.lock()->m_lockOnActor.expired()) return;

	// アニメーションを遷移
	m_player.lock()->m_model->ChangeAnimation(kMoveAnimName);
}

PlayerMove::~PlayerMove()
{
}

std::shared_ptr<PlayerState> PlayerMove::Update()
{
	Input& input = Input::GetInstance();
	auto p = m_player.lock();

	// この状態に遷移した時、もしロックオンされていたら
	if (!m_player.lock()->m_lockOnActor.expired())
	{
		// ロックオンの方に遷移してあげる
		// 結局条件分岐に頼ることになるな
		// まあ攻撃状態を両方に使い回してるのが悪いか

		// ここに四方向の移動状態への遷移条件を書きたくないので、いったんLockOnIdle状態に移行する
		// すると攻撃→Idle→Moveと二回アニメーションが切り替わるので、アニメーション補完が死ぬ。
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}

	// 移動
	p->Move(kRunSpeed);

	// 入力が切れたら待機状態へ
	if (input.GetLeftInputAxis().SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	if (input.IsTrigger("Attack"))
	{
		// 移動時に攻撃を入力するとちょっと追加で前進する
		p->GetCollidable().AddVel(VTransformSR({0,0,kSashAttackForce}, p->GetModelMatrix()));

		return std::make_shared<PlayerSlashDown>(m_player);
	}

	++m_moveFrame;

	return shared_from_this();
}
