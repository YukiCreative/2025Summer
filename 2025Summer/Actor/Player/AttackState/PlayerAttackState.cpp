#include "PlayerAttackState.h"
#include "../../../General/Input.h"
#include "../Player.h"
#include "../NormalState/PlayerIdle.h"
#include "../../../Physics/Rigid.h"
#include "../../ActorController.h"
#include "../../../Geometry/Geometry.h"
#include "../../../Camera/Camera.h"
#include "../../../Model/AnimationModel.h"
#include "../NormalState/PlayerMove.h"

namespace
{ 
	constexpr float kTrackEnemyDistance = 500.0f;
}

PlayerAttackState::PlayerAttackState(std::weak_ptr<Player> parent) :  
    PlayerState(parent),  
    m_isEnterAttack(false),  
    m_frame(0),  
    m_comboAttack(nullptr),  
    m_acceptNextAttackInputFrame(0),  
    m_attackPower(0.0f),  
    m_disableAttackColFrame(0),  
    m_enableAttackColFrame(0),  
    m_enableComboFrame(0),  
    m_enemyTrackForce(0.0f),  
    m_isLoopAnim(false),  
    m_knockbackPower(0.0f),  
    m_stateTotalFrame(0),  
    m_trackForce(0.0f),  
    m_trackFrame(0)  
{  
	// ロックオンをまたぐ
	m_canCrossState = true;
}

PlayerAttackState::~PlayerAttackState()
{
}

void PlayerAttackState::PlayAnim()
{
	m_player.lock()->m_model->ChangeAnimation(m_animName, m_isLoopAnim);
}

std::shared_ptr<PlayerState> PlayerAttackState::Update()
{
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	// 剣の攻撃判定を有効化
	if (m_frame == m_enableAttackColFrame)
	{
		p->EnableSwordCol(m_attackPower, m_knockbackPower);
		p->SetActionKind(m_actionKind);
	}
	if (m_frame == m_disableAttackColFrame)
	{
		p->DisableSwordCol();
	}

	// 敵に吸いつく挙動or前進
	if (m_frame == m_trackFrame)
	{
		p->GetRigid().AddVel(TrackingVec(m_trackForce, m_enemyTrackForce));
	}

	// 先行して入力をとっておく
	if (m_frame >= m_acceptNextAttackInputFrame)
	{
		m_isEnterAttack |= input.IsTrigger("Attack");
	}

	// 次の攻撃へ
	// ネスト深いな
	if (m_frame >= m_enableComboFrame)
	{
		if (m_isEnterAttack)
		{
			// 派生先が設定されていれば
			if (m_comboAttack)
			{
				// 初期化して返す
				m_comboAttack->Init();
				m_comboAttack->PlayAnim();
				return m_comboAttack;
			}
		}

		// 移動していたら
		if (input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
		{
			p->DisableSword();

			// 移動状態へ移行
			// ロックオン状態でも正常に動作するようになっているので心配なく
			return std::make_shared<PlayerMove>(m_player);
		}
	}

	// 待機状態へ遷移
	if (m_frame >= m_stateTotalFrame)
	{
		p->DisableSword();

		return std::make_shared<PlayerIdle>(m_player);
	}

	// 任意の処理
	OptionalProcess();

	++m_frame;

	return shared_from_this();
}

Vector3 PlayerAttackState::TrackingVec(const float strength, const float trackEnemyStrength)
{
	Vector3 vel;

	vel = TrackVec(strength, trackEnemyStrength);

	RotatePlayer(vel);

	return vel;
}

Vector3 PlayerAttackState::TrackVec(const float strength, const float trackEnemyStrength)
{
	const auto& inputAxis = Input::GetInstance().GetLeftInputAxis();
	auto p = m_player.lock();

	// 入力があれば
	if (inputAxis.SqrMagnitude() > kMoveThreshold && p->m_lockOnActor.expired())
	{
		Vector3 inputV = p->m_camera.lock()->RotateVecToCameraDirXZ({ -inputAxis.x, 0, inputAxis.y, }, Vector3::Back());
		// 入力の向きに前進
		return inputV.GetNormalize() * strength;
	}
		
	// ロックオンしてるなら、その敵の方向
	if (p->IsLockOn())
	{
		return (p->m_lockOnActor.lock()->GetPos() - p->GetPos()).GetNormalize() * trackEnemyStrength;
	}

	auto nearestActor = p->m_cont.lock()->SearchNearestLockOnActor(m_player.lock()->GetPos());
	// そうでなければ近くの敵の方向
	if (nearestActor && (nearestActor->GetPos() - p->GetPos()).SqrMagnitude() < kTrackEnemyDistance * kTrackEnemyDistance)
	{
		return (nearestActor->GetPos() - p->GetPos()).GetNormalize() * trackEnemyStrength;
	}

	// さらに一定範囲内に敵もいなかったら現在のモデルの向きに前進
	return VTransformSR({ 0,0,strength }, m_player.lock()->GetModelMatrix());
}

void PlayerAttackState::RotatePlayer(const Vector3& vec)
{
	auto p = m_player.lock();

	auto c = Geometry::Corner(p->m_model->GetDirection(), vec.XZ());

	// 符号をつける
	if (p->m_model->GetDirection().Cross(vec.XZ()).y < 0)
	{
		c *= -1;
	}

	// モデルを回転させる
	p->m_model->RotateUpVecY(c);
}
