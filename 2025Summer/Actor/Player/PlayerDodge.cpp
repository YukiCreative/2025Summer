#include "PlayerDodge.h"
#include "Player.h"
#include "../../Physics/Rigid.h"
#include "../../Model/AnimationModel.h"
#include <DxLib.h>
#include "NormalState/PlayerIdle.h";
#include "../../GameManagement/Score/StylishRank.h"
#include "../../General/Input.h"
#include "NormalState/PlayerMove.h"

namespace
{
	constexpr int kInvincibleStart = 0;
	constexpr int kInvincibleTime = 20;

	constexpr int kJustDodgeFrame = 10;

	const Vector3 kDodgeForce = { 0.0f, 0.0f, 4.0f };
	constexpr int kForceFrame = 25;

	constexpr int kCanMoveFrame = 30;

	const std::string kAnimName =  "Armature|RollingDodge";
	const std::string kJustAnimName =  "Armature|JustDodge";
}

PlayerDodge::PlayerDodge(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0),
	m_isJustDodge(false)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);
}

PlayerDodge::~PlayerDodge()
{
}

std::shared_ptr<PlayerState> PlayerDodge::Update()
{
	auto p = m_player.lock();

	// 状態遷移後から一定時間前方に移動
	if (m_frame < kForceFrame)
	{
		p->GetRigid().AddVel(VTransformSR(kDodgeForce, p->GetModelMatrix()));
	}

	// モーションを流す
	// 特定フレームで無敵
	if (m_frame == kInvincibleStart)
	{
		p->SetInvincibility(true);
	}
	if (m_frame == kInvincibleStart + kInvincibleTime)
	{
		p->SetInvincibility(false);
	}

	++m_frame;

	// 特定フレームの間に攻撃を受けるとジャスト回避
	if (p->m_isDamageFromEnemy && m_frame < kJustDodgeFrame)
	{
		JustDodge();
	}
	if (p->m_model->IsEnd())
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	if (m_frame > kCanMoveFrame && Input::GetInstance().GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
	{
		return std::make_shared<PlayerMove>(m_player);
	}

	return shared_from_this();
}

void PlayerDodge::JustDodge()
{
	if (m_isJustDodge) return;

	m_player.lock()->SetStopFrame(10);

	m_isJustDodge;

	m_player.lock()->ChangeAnim(kJustAnimName, false);
	// スタイリッシュゲージを増加
	StylishRank::GetInstance().IncreaseStylishPoint(IncreaseStylishPointKind::kJustDodge);
	// この後アニメーションが終わるまで無敵

}
