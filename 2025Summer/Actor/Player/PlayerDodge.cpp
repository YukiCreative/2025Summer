#include "PlayerDodge.h"
#include "Player.h"
#include "../../Physics/Rigid.h"
#include "../../Model/AnimationModel.h"
#include <DxLib.h>
#include "NormalState/PlayerIdle.h"
#include "../../GameManagement/Score/StylishRank.h"
#include "../../General/Input.h"
#include "NormalState/PlayerMove.h"
#include "../../Camera/Camera.h"
#include "../../Geometry/Geometry.h"
#include "AttackState/PlayerSlashDown.h"
#include "../../Sound/SoundManager.h"

namespace
{
	constexpr int kInvincibleStart = 0;
	constexpr int kInvincibleTime = 20;

	constexpr int kJustDodgeFrame = 10;

	const Vector3 kDodgeForce = { 0.0f, 0.0f, 4.0f };
	constexpr int kForceFrame = 25;

	constexpr int kTypeAheadFrame = 20;
	constexpr int kCanTransitionFrame = 35;

	const std::string kStunAnimName = "Armature|RollingDodge";
	const std::string kJustAnimName = "Armature|JustDodge";
	const std::string kDodgeSound = "Dodge.wav";
	const std::string kJustDodgeSound = "JustDodge.mp3";
}

PlayerDodge::PlayerDodge(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0),
	m_isJustDodge(false),
	m_isInputAheadAttack(false),
	m_isInputAheadDodge(false)
{
	Input& input = Input::GetInstance();
	std::shared_ptr<Player> p = m_player.lock();

	m_canCrossState = true;

	p->m_model->ChangeAnimation(kStunAnimName, false);
	// 入力方向に向きを変える

	Vector3 inputAxis = Vector3{ input.GetLeftInputAxis().x, 0, input.GetLeftInputAxis().y };
	inputAxis.z *= -1;
	Vector3 cameraRotatedAxis = p->m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Foward());
	const Vector3 cameraRotatedAxisN = cameraRotatedAxis.GetNormalize();

	// キャラの向きに対して入力がどんな位置関係か調べたい
	const Vector3 cross = p->GetDirection().Cross(cameraRotatedAxisN);
	float corner = Geometry::Corner(cameraRotatedAxisN, p->GetDirection());

	if (cross.y < 0) corner *= -1;

	p->m_model->RotateUpVecY(corner);
	p->DisableSword();
	SoundManager::GetInstance().Play(kDodgeSound);
}

PlayerDodge::~PlayerDodge()
{
}

std::shared_ptr<PlayerState> PlayerDodge::Update()
{
	auto p = m_player.lock();
	Input& input = Input::GetInstance();

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
		p->SetInvincibility(false);
		return std::make_shared<PlayerIdle>(m_player);
	}
	if (m_frame > kTypeAheadFrame)
	{
		m_isInputAheadDodge |= input.IsTrigger("Dodge");
		m_isInputAheadAttack |= input.IsTrigger("Attack");
	}
	if (m_frame > kCanTransitionFrame)
	{
		if (input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
		{
			return std::make_shared<PlayerMove>(m_player);
		}
		if (m_isInputAheadDodge)
		{
			return std::make_shared<PlayerDodge>(m_player);
		}
		if (m_isInputAheadAttack)
		{
			return std::make_shared<PlayerSlashDown>(m_player);
		}
	}

	return shared_from_this();
}

void PlayerDodge::JustDodge()
{
	if (m_isJustDodge) return;

	auto p = m_player.lock();

	p->SetStopFrame(10);

	m_isJustDodge;

	p->ChangeAnim(kJustAnimName, false);
	// スタイリッシュゲージを増加
	StylishRank::GetInstance().IncreaseStylishPoint(IncreaseStylishPointKind::kJustDodge);
	// この後アニメーションが終わるまで無敵
	p->SetInvincibility(true);
	// 音出す
	SoundManager::GetInstance().Play(kJustDodgeSound);
}
