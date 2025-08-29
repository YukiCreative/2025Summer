#include "EnemyElite.h"
#include "EnemyEliteState.h"
#include "EnemyEliteIdle.h"
#include "EnemyEliteStun.h"
#include "EnemyEliteDeath.h"
#include "../../Player/Player.h"
#include "../../../Physics/Collider/CapsuleCollider.h"
#include "../../../Physics/Rigid.h"
#include "../../../Physics/Collidable.h"
#include "../../../Model/AnimationModel.h"
#include "../../../Utility/MyRandom.h"
#include "../../AttackCol.h"
#include "EnemyEliteBiteCol.h"
#include "EnemyEliteArmCol.h"
#include <DxLib.h>
#include "../../../Geometry/Geometry.h"
#include "EnemyEliteKnockUp.h"

namespace
{
	constexpr float kMaxHp = 2000.0f;
	const Vector2 kDrag = { 0.1f, 0.02f };
	const Vector3 kCapsuleEndOffset = {0, 100, 0};
	constexpr int kWeight = 50;
	constexpr float kRadius = 160.0f;
	const Vector3 kModelScale = {2.0f, 2.0f, 2.0f};

	const std::string kBiteFrame = "sloth_tongue04";
	const std::string kRightArmStartFrame = "sloth_RArm2";
	const std::string kRightArmEndFrame = "sloth_RArmDigit31";
	const std::string kLeftArmStartFrame = "sloth_LArm2";
	const std::string kLeftArmEndFrame = "sloth_LArmDigit31";

	constexpr int kAttackFrame = 180;
	constexpr int kRandomness = 10;

	constexpr float kKnockbackMult = 0.5f;

	constexpr int kBloodFrame = 5;

	constexpr float kMaxStunPoint = 30.0f;
}

std::normal_distribution<> EnemyElite::s_attackIntervalNormalDist(kAttackFrame, kRandomness);

EnemyElite::EnemyElite() :
	Enemy(kMaxStunPoint, kMaxHp)
{
}

void EnemyElite::Init(const std::weak_ptr<Player> player, const Vector3& initPos, const int dupulicatedHandle)
{
	Enemy::Init(player, initPos, dupulicatedHandle);

	m_enemyKind = EnemyKind::kElite;
	m_bloodFrameIndex = kBloodFrame;

	m_model->SetScale(kModelScale);
	// âÒì]
	m_model->RotateUpVecY(Geometry::kPi);

	// ìñÇΩÇËîªíË
	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + kCapsuleEndOffset, kWeight, false, false, kRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	// èâä˙èÛë‘
	m_state = std::make_shared<EnemyEliteIdle>(weak_from_this());
}

void EnemyElite::UpdateState()
{
	m_state = m_state->Update();
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

bool EnemyElite::CheckAnimName(const std::string& animName) const
{
	return m_model->CheckAnimName(animName);
}

void EnemyElite::BiteAttack()
{
	auto col = std::make_shared<EnemyEliteBiteCol>();
	col->Init(weak_from_this());

	SpawnActor(col);
}

void EnemyElite::ArmSwingAttack()
{
	// ìÒÇ¬ê∂ê¨
	auto rightCol = std::make_shared<EnemyEliteArmCol>();
	rightCol->Init(weak_from_this(), LeftOrRight::kRight);
	SpawnActor(rightCol);

	auto leftCol = std::make_shared<EnemyEliteArmCol>();
	leftCol->Init(weak_from_this(), LeftOrRight::kLeft);
	SpawnActor(leftCol);
}

Vector3 EnemyElite::GetBiteRigPos() const
{
	return m_model->GetFramePosition(kBiteFrame);
}

Vector3 EnemyElite::GetArmRightStartPos() const
{
	return m_model->GetFramePosition(kRightArmStartFrame);
}

Vector3 EnemyElite::GetArmRightEndPos() const
{
	return m_model->GetFramePosition(kRightArmEndFrame);
}

Vector3 EnemyElite::GetArmLeftStartPos() const
{
	return m_model->GetFramePosition(kLeftArmStartFrame);
}

Vector3 EnemyElite::GetArmLeftEndPos() const
{
	return m_model->GetFramePosition(kLeftArmEndFrame);
}

int EnemyElite::GetAttackInterval()
{
	return static_cast<int>(s_attackIntervalNormalDist(MyRandom::GetInstance().GetRandomEngine()));
}

void EnemyElite::OnDamage(std::weak_ptr<PlayerAttackCol> attack)
{
	// ñ≥ìGÇ»ÇÁêHÇÁÇÌÇ»Ç¢
	if (m_isInvincible) return;

	Enemy::OnDamage(attack);

	if (m_hitPoint.IsMin())
	{
		m_state = std::make_shared<EnemyEliteDeath>(weak_from_this());
		return;
	}

	// Ç∑Ç≈Ç…ÉXÉ^ÉìÇ‚ë≈Çøè„Ç∞èÛë‘Ç»ÇÁëJà⁄ÇµÇ»Ç¢
	if (typeid(*m_state) == typeid(EnemyEliteStun) ||
		typeid(*m_state) == typeid(EnemyEliteKnockUp))
	{
		return;
	}

	if (m_stunPoint.IsStun())
	{
		// ÉXÉ^Éì
		m_state = std::make_shared<EnemyEliteStun>(weak_from_this());
		return;
	}
}