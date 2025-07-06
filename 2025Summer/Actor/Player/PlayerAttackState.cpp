#include "PlayerAttackState.h"
#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "Rigid.h"
#include "ActorController.h"
#include "Geometry.h"
#include "Camera.h"
#include "AnimationModel.h"
#include "PlayerMove.h"

PlayerAttackState::PlayerAttackState(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_isEnterAttack(false),
	m_frame(0),
	m_comboAttack(nullptr)
{
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
		p->EnableSwordCol(m_attackPower);
	}
	if (m_frame == m_disableAttackColFrame)
	{
		p->DisableSwordCol();
	}

	// 敵に吸いつく挙動or前進
	if (m_frame == m_trackFrame)
	{
		p->GetRigid().AddVel(TrackingVec(m_trackForce));
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

Vector3 PlayerAttackState::TrackingVec(const float strength)
{
	Vector3 vel;
	// 入力があったら、その方向に動く
	const auto& inputAxis = Input::GetInstance().GetLeftInputAxis();
	auto p = m_player.lock();

	// ロックオンされておらず、入力があれば
	if (inputAxis.SqrMagnitude() > kMoveThreshold && p->m_lockOnActor.expired())
	{
		// 入力をカメラに回転
		Vector3 inputV = p->m_camera.lock()->RotateVecToCameraDirXZ({ -inputAxis.x, 0, inputAxis.y, }, Vector3::Back());

		vel = inputV.GetNormalize() * strength;
	}
	else
	{
		// そうでなければ近くの敵の方向
		auto nearestActor = p->m_cont.lock()->GetNearestLockOnActor(m_player.lock()->GetPos());
		auto posToNearest = nearestActor->GetPos() - p->GetPos();
		if (posToNearest.SqrMagnitude() < 100000)
		{
			vel = posToNearest.GetNormalize() * strength;
		}
		else
		{
			// さらに一定範囲内に敵もいなかったら現在のモデルの向きに前進
			vel = VTransformSR({ 0,0,strength }, m_player.lock()->GetModelMatrix());
		}
	}

	auto c = Geometry::Corner(p->m_model->GetDirection(), vel.XZ());

	// 符号をつける
	if (p->m_model->GetDirection().Cross(vel.XZ()).y < 0)
	{
		c *= -1;
	}

	// モデルを回転させる
	p->m_model->RotateUpVecY(c);

	return vel;
}