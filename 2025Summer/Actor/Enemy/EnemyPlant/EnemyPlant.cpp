#include "AnimationModel.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "EnemyPlant.h"
#include "EnemyPlantBullet.h"
#include "EnemyPlantDamage.h"
#include "EnemyPlantDeath.h"
#include "EnemyPlantIdle.h"
#include "Player.h"
#include "Rigid.h"
#include <DxLib.h>
#include "MyRandom.h"

namespace
{
	constexpr float kInitHP = 750.0f;

	// コライダー
	const Vector3 kCapsuleEndOffset = {0.0f, 100.0f, 0.0f};
	constexpr int kWeight = 10;
	constexpr float kRadius = 80.0f;
	const Vector3 kModelExRate = { 1.5f,1.5f,1.5f };

	constexpr float kDrag = 0.1f;

	constexpr int kAttackFrame = 280;
	constexpr int kRandomness = 30;

	const std::string kBulletSpawnFrame = "plant_tongue_05";

	constexpr int kBloodFrame = 38;
}

std::normal_distribution<> EnemyPlant::s_attackIntervalNormalDist(kAttackFrame, kRandomness);

EnemyPlant::EnemyPlant()
{
}

void EnemyPlant::Init(const std::weak_ptr<Player> player, const Vector3& initPos, const int dupulicatedHandle)
{
	Enemy::Init(player, initPos, kInitHP, dupulicatedHandle);

	m_enemyKind = EnemyKind::kPlant;
	m_bloodFrameIndex = kBloodFrame;

	// モデル
	m_model->SetScale(kModelExRate);

	// 当たり判定
	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + kCapsuleEndOffset, kWeight, false, false, kRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	// 初期状態
	m_state = std::make_shared<EnemyPlantIdle>(weak_from_this());
}

void EnemyPlant::UpdateState()
{
	m_state = m_state->Update();
}

void EnemyPlant::CommitMove()
{
	m_pos += m_collidable->UpdateRigid();

	m_collidable->SetPos(m_pos + kCapsuleEndOffset);
	m_model->SetPos(m_pos);
}

void EnemyPlant::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	if (other->GetKind() == ActorKind::kPlayerAttack)
	{
		OnDamage(std::static_pointer_cast<AttackCol>(other));
	}
}

void EnemyPlant::OnDeath()
{
	Destroy();
}

void EnemyPlant::SpawnBullet()
{
	auto bullet = std::make_shared<EnemyPlantBullet>();
	bullet->Init(m_model->GetFramePosition(kBulletSpawnFrame), GetModelMatrix());
	// 弾クラスを生成
	SpawnActor(bullet);
}

int EnemyPlant::GetAttackInterval()
{
	return static_cast<int>(s_attackIntervalNormalDist(MyRandom::GetInstance().GetRandomEngine()));
}

void EnemyPlant::OnDamage(std::shared_ptr<AttackCol> attack)
{
	// 無敵なら食らわない
	if (m_isInvincible) return;

	auto attackPower = attack->GetAttackPower();

#if _DEBUG
	printf("食らった！%fダメージ！\n", attackPower);
#endif

	m_hitPoint -= attackPower;

	if (m_hitPoint.IsMin())
	{
		m_state = std::make_shared<EnemyPlantDeath>(weak_from_this());
	}
	else
	{
		m_state = std::make_shared<EnemyPlantDamage>(weak_from_this());
	}

	// 食らった当たり判定の位置を見て吹っ飛ぶ
	auto colToEN = (m_pos.XZ() - attack->GetPos().XZ()).GetNormalize();
	m_collidable->SetVel(VTransformSR({ 0,0, attack->GetKnockbackPower() }, MGetRotVec2(Vector3::Foward(), colToEN)));
}
