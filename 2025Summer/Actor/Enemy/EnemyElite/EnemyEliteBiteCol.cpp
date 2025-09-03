#include "EnemyEliteBiteCol.h"
#include <string>
#include "EnemyElite.h"
#include "../../../Physics/Collider/SphereCollider.h"
#include <DxLib.h>
#include "../../../Physics/Rigid.h"
#include "../../../Physics/Collidable.h"
#include "../../../Sound/SoundManager.h"
#include "../../Player/Player.h"

namespace
{
	const std::string kTrajectoryEffectName = ".efkefc";
	constexpr float kRadius = 50.0f;
	constexpr float kLifeTime = 40.0f;
	constexpr float kLength = 10.0f;
	constexpr float kAttackPower = 100.0f;
	const Vector3 kKnockbackPower = { 0.0f, 0.0f, 30.0f };
	const Vector2 kDrag = { 0.0f, 0.0f };

	const std::string kSound = "EliteBite.wav";
}

EnemyEliteBiteCol::EnemyEliteBiteCol() :
	m_frame(0)
{
}

EnemyEliteBiteCol::~EnemyEliteBiteCol()
{
}

void EnemyEliteBiteCol::Init(std::weak_ptr<EnemyElite> parent)
{
	m_kind = ActorKind::kEnemyAttack;

	m_parent = parent;

	m_attackPower = kAttackPower;
	m_knockbackPower = kKnockbackPower;

	m_pos = m_parent.lock()->GetBiteRigPos();

	auto capsuleOffset = VTransformSR({ kLength,0,0 }, m_parent.lock()->GetModelMatrix());

	auto col = std::make_shared<SphereCollider>();
	col->Init(m_pos - capsuleOffset, 0, true, true, kRadius);

	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);
}

void EnemyEliteBiteCol::Update()
{
	m_pos = m_parent.lock()->GetBiteRigPos();

	if (m_frame > kLifeTime)
	{
		Destroy();
	}

	++m_frame;
}

void EnemyEliteBiteCol::Draw() const
{
#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
}

void EnemyEliteBiteCol::CommitMove()
{
	m_collidable->SetPos(m_pos);
}

void EnemyEliteBiteCol::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	// プレイヤーが食らったら音を鳴らす
	if (other->GetKind() != ActorKind::kPlayer) return;

	if (std::static_pointer_cast<Player>(other)->IsInvincible()) return;

	SoundManager::GetInstance().Play(kSound);
}
