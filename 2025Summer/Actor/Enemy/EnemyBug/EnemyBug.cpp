#include "../../../Model/AnimationModel.h"
#include "../../AttackCol.h"
#include "../../../Physics/Collider/CapsuleCollider.h"
#include "../../../Physics/Collidable.h"
#include "EnemyBug.h"
#include "EnemyBugAttack.h"
#include "EnemyBugAttackCol.h"
#include "EnemyBugDamage.h"
#include "EnemyBugDeath.h"
#include "EnemyBugIdle.h"
#include "EnemyBugState.h"
#include "../../../Utility/MyRandom.h"
#include "../../Player/Player.h"
#include "../../../Physics/Rigid.h"
#include "EnemyBugStun.h"
#include "EnemyBugKnockUp.h"
#include <DxLib.h>
#include <typeinfo>

namespace
{
	const Vector2 kDrag = { 0.1f, 0.01f };

	// コライダー
	constexpr float kSphereRadius = 60.0f;
	constexpr int kWeight = 10;

	const Vector3 kColOffset = {0,50,0};

	// 攻撃判定を出現させる時に基準にするリグ
	const std::string kCollisionFrameName1 = "bug_mandible_R";
	const std::string kCollisionFrameName2 = "bug_mandible_L";

	// 乱数
	constexpr int kAttackFrame = 300;
	constexpr int kRandomness = 60;

	constexpr float kMaxHP = 800.0f;

	constexpr int kBloodFrame = 5;

	constexpr float kMaxStunPoint = 10.0f;

	const Vector3 kCapsuleLength = { 0, 50, 0 };
}

std::normal_distribution<> EnemyBug::s_attackTimeNormalDist(kAttackFrame, kRandomness);

EnemyBug::EnemyBug() :
	Enemy(kMaxStunPoint, kMaxHP),
	m_attackFrame(0)
{
}

void EnemyBug::Init(std::weak_ptr<Player> player, const Vector3& initPos, const int dupulicatedHandle)
{
	Enemy::Init(player, initPos, dupulicatedHandle);

	m_enemyKind = EnemyKind::kBug;
	m_bloodFrameIndex = kBloodFrame;

	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + kCapsuleLength, kWeight, false, false, kSphereRadius);
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

	// DEBUG
	if (vel.y > 50.0f)
	{
		if (vel.y > 100.0f) 
		{

		}
	} 
	// DEBUG

	m_pos += vel;

	m_model->SetPos(m_pos);
	m_collidable->SetPos(m_pos + kColOffset);
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
	if (!this) return Vector3::Zero();

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

void EnemyBug::OnDamage(std::weak_ptr<PlayerAttackCol> attack)
{
	// 無敵なら食らわない
	if (m_isInvincible) return;

	Enemy::OnDamage(attack);

	if (m_hitPoint.IsMin())
	{
		m_state = std::make_shared<EnemyBugDeath>(weak_from_this());
		return;
	}

	// すでにスタンや打ち上げ状態なら遷移しない
	// 苦渋の型比較
	if (typeid(*m_state) == typeid(EnemyBugStun) ||
		typeid(*m_state) == typeid(EnemyBugKnockUp))
	{
		return;
	}

	if (m_stunPoint.IsStun())
	{
		// スタン
		m_state = std::make_shared<EnemyBugStun>(weak_from_this());
		return;
	}
}