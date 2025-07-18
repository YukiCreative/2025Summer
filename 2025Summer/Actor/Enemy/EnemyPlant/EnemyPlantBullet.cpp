#include "EnemyPlantBullet.h"
#include <DxLib.h>
#include "Collidable.h"
#include "SphereCollider.h"
#include "Rigid.h"
#include "EffectManager.h"
#include "EffekseerEffect.h"
#include "Player.h"

namespace
{
	constexpr int kRadius = 50.0f;
	const Vector3 kMoveSpeed = { 0,0, 1.0f };
	constexpr int kLifeTime = 300;

	const std::string kTrajectoryEffectName = "FireBall.efkefc";

	constexpr float kDamage = 100.0f;
	constexpr float kKnockback = 10.0f;

	constexpr float kDrag = 0.1f;
}

EnemyPlantBullet::EnemyPlantBullet() :
	m_frame(0)
{
}

EnemyPlantBullet::~EnemyPlantBullet()
{
	if (m_effect.expired()) return;

	m_effect.lock()->Kill();
}

void EnemyPlantBullet::Init(const Vector3& initPos, const DxLib::tagMATRIX& dir)
{
	m_kind = ActorKind::kEnemyAttack;

	m_pos = initPos;
	m_attackPower = kDamage;
	m_knockbackPower = kKnockback;
	m_dir = std::make_shared<MATRIX>(dir);

	auto col = std::make_shared<SphereCollider>();
	col->Init(m_pos, 0, true, true, kRadius);

	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	// エフェクト出す
	m_effect = EffectManager::GetInstance().GenerateEffect(kTrajectoryEffectName, m_pos);
	m_effect.lock()->SetScale({0.5f,0.5f,0.5f});
}

void EnemyPlantBullet::Update()
{
	// 前進
	auto vel = VTransformSR(kMoveSpeed, *m_dir);
	m_collidable->AddVel(vel);

	if (m_frame > kLifeTime)
	{
		m_isAlive = false;
	}

	++m_frame;
}

void EnemyPlantBullet::Draw() const
{
	// エフェクトが出ている
#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
}

void EnemyPlantBullet::CommitMove()
{
	m_pos += m_collidable->UpdateRigid();

	// エフェクト、当たり判定の位置設定
	m_effect.lock()->SetPos(m_pos);
	m_collidable->SetPos(m_pos);
}

void EnemyPlantBullet::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	// 障害物やプレイヤーに触れたら消える
	if (other->GetKind() == ActorKind::kPlayer)
	{
		// プレイヤーが無敵なら消滅しない
		if (std::static_pointer_cast<Player>(other)->IsInvincible()) return;

		m_isAlive = false;
	}
}
