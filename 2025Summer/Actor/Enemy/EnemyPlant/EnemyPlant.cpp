#include "AnimationModel.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "EnemyPlant.h"
#include "EnemyPlantBullet.h"
#include "EnemyPlantDamage.h"
#include "EnemyPlantDeath.h"
#include "EnemyPlantIdle.h"
#include "MyRandom.h"
#include "Player.h"
#include "Rigid.h"
#include <DxLib.h>

namespace
{
	constexpr float kInitHP = 750.0f;
	const std::string kModelName = "Data/Model/Plant.mv1";

	constexpr float kAnimSpeed = 30.0f;

	// コライダー
	const Vector3 kCapsuleEndOffset = {0.0f, 100.0f, 0.0f};
	constexpr int kWeight = 10;
	constexpr float kRadius = 80.0f;
	const Vector3 kModelExRate = { 1.5f,1.5f,1.5f };

	const PhysicalMaterial kMat =
	{
		0.1f, 0.05f, 0.1f
	};

	constexpr int kAttackFrame = 200;
	constexpr int kRandomness = 30;

	const std::string kBulletSpawnFrame = "plant_tongue_05";
}

std::normal_distribution<> EnemyPlant::s_attackIntervalNormalDist(kAttackFrame, kRandomness);

EnemyPlant::EnemyPlant()
{
}

void EnemyPlant::Init(const std::weak_ptr<Player> player, const Vector3& initPos)
{
	Enemy::Init(player, initPos, kInitHP);

	// モデル
	m_model = std::make_shared<AnimationModel>();
	m_model->Init(kModelName, kAnimSpeed);
	m_model->SetScale(kModelExRate);

	// 当たり判定
	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + kCapsuleEndOffset, kWeight, false, false, kRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kMat);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	// 初期状態
	m_state = std::make_shared<EnemyPlantIdle>(weak_from_this());
}

void EnemyPlant::Update()
{
	m_state = m_state->Update();

	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	m_model->Update();
}

void EnemyPlant::Draw() const
{
	m_model->Draw();

#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
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
	m_isAlive = false;
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
	auto attackPower = attack->GetAttackPower();

	if (m_isInvincible) return;

	printf("食らった！%fダメージ！\n", attackPower);

	m_hitPoint -= attackPower;

	if (m_hitPoint.IsMin())
	{
		m_state = std::make_shared<EnemyPlantDeath>(weak_from_this());
	}
	else
	{
		m_state = std::make_shared<EnemyPlantDamage>(weak_from_this());
	}

	// プレイヤーの位置を見て吹っ飛ぶ
	auto pToEN = (m_pos - m_player.lock()->GetPos()).GetNormalize();
	m_collidable->AddVel(VTransformSR({ 0,0, attack->GetKnockbackPower() }, MGetRotVec2(Vector3::Foward(), pToEN)));
}
