#include "EnemyElite.h"
#include "EnemyEliteState.h"
#include "EnemyEliteIdle.h"
#include "EnemyEliteDamage.h"
#include "EnemyEliteDeath.h"
#include "Player.h"
#include "CapsuleCollider.h"
#include "Rigid.h"
#include "Collidable.h"
#include "AnimationModel.h"
#include "MyRandom.h"
#include "AttackCol.h"
#include <DxLib.h>

namespace
{
	constexpr float kMaxHp = 2000.0f;
	constexpr float kDrag = 0.1f;
	const Vector3 kCapsuleEndOffset = {0, 200, 0};
	constexpr int kWeight = 50;
	constexpr float kRadius = 160.0f;
	const Vector3 kModelScale = {2.0f, 2.0f, 2.0f};

	const std::string kBiteFrame = "sloth_tongue04";
	const std::string kRightArmFrame = "sloth_RArmPalm";
	const std::string kLeftArmFrame = "sloth_LArmPalm";

	constexpr int kAttackFrame = 180;
	constexpr int kRandomness = 10;

	constexpr float kKnockbackMult = 0.5f;
}

std::normal_distribution<> EnemyElite::s_attackIntervalNormalDist(kAttackFrame, kRandomness);

EnemyElite::EnemyElite()
{
}

void EnemyElite::Init(const std::weak_ptr<Player> player, const Vector3& initPos, const int dupulicatedHandle)
{
	Enemy::Init(player, initPos, kMaxHp, dupulicatedHandle);

	m_model->SetScale(kModelScale);

	// 当たり判定
	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + kCapsuleEndOffset, kWeight, false, false, kRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	// 初期状態
	m_state = std::make_shared<EnemyEliteIdle>(weak_from_this());

	m_hitPoint = kMaxHP;
}

void EnemyElite::Update()
{
	m_state = m_state->Update();

	m_model->Update();
}

void EnemyElite::Draw() const
{
	m_model->Draw();

#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
}

void EnemyElite::CommitMove()
{
	m_pos += m_collidable->UpdateRigid();

	m_collidable->SetPos(m_pos + kCapsuleEndOffset);
	m_model->SetPos(m_pos);
}

void EnemyElite::OnDeath()
{
	Destroy();
}

void EnemyElite::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	if (other->GetKind() == ActorKind::kPlayerAttack)
	{
		auto attack = std::static_pointer_cast<AttackCol>(other);

		OnDamage(attack);
	}
}

bool EnemyElite::CheckAnimName(const std::string& animName) const
{
	return m_model->CheckAnimName(animName);
}

void EnemyElite::BiteAttack()
{
}

void EnemyElite::ArmSwingAttack()
{
}

Vector3 EnemyElite::GetBiteRigPos() const
{
	return m_model->GetFramePosition(kBiteFrame);
}

Vector3 EnemyElite::GetArmRightPos() const
{
	return m_model->GetFramePosition(kRightArmFrame);
}

Vector3 EnemyElite::GetArmLeftPos() const
{
	return  m_model->GetFramePosition(kLeftArmFrame);
}

int EnemyElite::GetAttackInterval()
{
	return static_cast<int>(s_attackIntervalNormalDist(MyRandom::GetInstance().GetRandomEngine()));
}

void EnemyElite::OnDamage(std::shared_ptr<AttackCol> attack)
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
		m_state = std::make_shared<EnemyEliteDeath>(weak_from_this());
	}
	else
	{
		m_state = std::make_shared<EnemyEliteDamage>(weak_from_this());
	}

	// プレイヤーの位置を見て吹っ飛ぶ
	// ボスなのでちょっと動きにくい
	auto pToEN = (m_pos - m_player.lock()->GetPos()).GetNormalize();
	m_collidable->AddVel(VTransformSR({ 0,0, attack->GetKnockbackPower() * kKnockbackMult }, MGetRotVec2(Vector3::Foward(), pToEN)));
}
