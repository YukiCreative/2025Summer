#include "AnimationModel.h"
#include "AttackCol.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "EnemyBug.h"
#include "EnemyBugAttack.h"
#include "EnemyBugAttackCol.h"
#include "EnemyBugDamage.h"
#include "EnemyBugDeath.h"
#include "EnemyBugIdle.h"
#include "EnemyBugState.h"
#include "MyRandom.h"
#include "Player.h"
#include "Rigid.h"
#include "SphereCollider.h"
#include <DxLib.h>

namespace
{
	constexpr float kDrag = 0.1f;

	// コライダー
	constexpr float kSphereRadius = 60.0f;
	constexpr int kWeight = 10;

	const Vector3 kColOffset = {0,60,0};

	// 攻撃判定を出現させる時に基準にするリグ
	const std::string kCollisionFrameName1 = "bug_mandible_R";
	const std::string kCollisionFrameName2 = "bug_mandible_L";

	// 乱数
	constexpr int kAttackFrame = 300;
	constexpr int kRandomness = 60;

	constexpr float kInitHP = 800.0f;
}

std::normal_distribution<> EnemyBug::s_attackTimeNormalDist(kAttackFrame, kRandomness);

EnemyBug::EnemyBug() :
	Enemy(),
	m_attackFrame(0)
{
}

void EnemyBug::Init(std::weak_ptr<Player> player, const Vector3& initPos, const int dupulicatedHandle)
{
	Enemy::Init(player, initPos, kInitHP, dupulicatedHandle);

	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + Vector3{0, 100, 0}, kWeight, false, false, kSphereRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_state = std::make_shared<EnemyBugIdle>(weak_from_this());

	m_attackFrame = GetAttackInterval();
}

void EnemyBug::UpdateState()
{
	m_state = m_state->Update();
}

void EnemyBug::CommitMove()
{
	const Vector3 vel = m_collidable->UpdateRigid();
	m_pos += vel;

	m_model->SetPos(m_pos);
	m_collidable->SetPos(m_pos + kColOffset);
}

void EnemyBug::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	if (other->GetKind() == ActorKind::kPlayerAttack)
	{
		auto attack = std::static_pointer_cast<AttackCol>(other);

		// ダメージ
		OnDamage(attack);
		return;
	}
}

void EnemyBug::OnCollisionStay(std::shared_ptr<Actor> other)
{
	if (other->GetKind() == ActorKind::kPlayerAttack)
	{
	}
}

void EnemyBug::OnCollisionExit(std::shared_ptr<Actor> other)
{
	if (other->GetKind() == ActorKind::kPlayerAttack)
	{
	}
}

void EnemyBug::OnDeath()
{
	Destroy();
}

void EnemyBug::GenerateAttackCol()
{
	auto col = std::make_shared<EnemyBugAttackCol>();
	col->Init(weak_from_this());

	SpawnActor(col);
}

int EnemyBug::GetAinmTotalTime() const
{
	return m_model->GetAnimTotalTime();
}

Vector3 EnemyBug::GetAttackRigPos() const
{
	auto ago1 = m_model->GetFramePosition(kCollisionFrameName1);
	auto ago2 = m_model->GetFramePosition(kCollisionFrameName2);


	return (ago1 + ago2) * 0.5f;
}

int EnemyBug::GetAttackInterval()
{
	auto interval = static_cast<int>(s_attackTimeNormalDist(MyRandom::GetInstance().GetRandomEngine()));
#if _DEBUG
	printf("%d\n", interval);
#endif
	return interval;
}

void EnemyBug::OnDamage(std::shared_ptr<AttackCol> attack)
{
	// 無敵なら食らわない
	if (m_isInvincible) return;

	auto attackPower = attack->GetAttackPower();

	// ヒットストップをかける
	m_stopFrame = 10;

#if _DEBUG
	printf("食らった！%fダメージ！\n", attackPower);
#endif

	m_hitPoint -= attackPower;

	if (m_hitPoint.IsMin())
	{
		m_state = std::make_shared<EnemyBugDeath>(weak_from_this());
	}
	else
	{
		m_state = std::make_shared<EnemyBugDamage>(weak_from_this());
	}

	// 食らった当たり判定の位置を見て吹っ飛ぶ
	auto colToEN = (m_pos.XZ() - attack->GetPos().XZ()).GetNormalize();
	m_collidable->SetVel(VTransformSR({ 0,0, attack->GetKnockbackPower() }, MGetRotVec2(Vector3::Foward(), colToEN)));
}
