#include "EnemyElite.h"
#include "EnemyEliteState.h"
#include "EnemyEliteIdle.h"
#include "CapsuleCollider.h"
#include "Rigid.h"
#include "Collidable.h"
#include "AnimationModel.h"

namespace
{
	constexpr float kMaxHp = 2000.0f;
	constexpr float kDrag = 0.1f;
	const Vector3 kCapsuleEndOffset = {0, 200, 0};
	constexpr int kWeight = 50;
	constexpr float kRadius = 160.0f;
	const Vector3 kModelScale = {2.0f, 2.0f, 2.0f};

	const std::string kBiteFrame = "sloth_tongue04";
}

EnemyElite::EnemyElite()
{
}

void EnemyElite::Init(const std::weak_ptr<Player> player, const Vector3& initPos, const int dupulicatedHandle)
{
	Enemy::Init(player, initPos, kMaxHp, dupulicatedHandle);

	m_model->SetScale(kModelScale);

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
	m_isAlive = false;
}

void EnemyElite::OnCollisionEnter(std::shared_ptr<Actor> other)
{
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
