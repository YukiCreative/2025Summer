#include "EnemyEliteArmCol.h"
#include "EnemyElite.h"
#include "Collidable.h"
#include "CapsuleCollider.h"
#include "Rigid.h"

namespace
{
	constexpr float kRadius = 50.0f;
	constexpr float kAttackPower = 200.0f;
	constexpr float kKnockbackPower = 80.0f;
	constexpr float kDrag = 0.0f;
	constexpr int kLifeTime = 50;
}

EnemyEliteArmCol::EnemyEliteArmCol() :
	m_frame(0),
	m_LR(LeftOrRight::kLeft)
{
}

EnemyEliteArmCol::~EnemyEliteArmCol()
{
}

void EnemyEliteArmCol::Init(std::weak_ptr<EnemyElite> parent, const LeftOrRight LR)
{
	// いつものように当たり判定の設定とか
	m_kind = ActorKind::kEnemyAttack;

	m_parent = parent;

	m_attackPower = kAttackPower;
	m_knockbackPower = kKnockbackPower;

	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos, 0, true, true, kRadius);

	m_LR = LR;
	SetCapsulePos();

	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

}

void EnemyEliteArmCol::Update()
{
	SetCapsulePos();

	if (m_frame > kLifeTime)
	{
		Destroy();
	}

	++m_frame;
}

void EnemyEliteArmCol::Draw() const
{
#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
}

void EnemyEliteArmCol::CommitMove()
{
	auto& capsule = static_cast<CapsuleCollider&>(m_collidable->GetCol());
	// 始点と終点を設定
	capsule.SetPos(m_pos, m_endPos);
}

void EnemyEliteArmCol::SetCapsulePos()
{
	auto parent = m_parent.lock();

	// m_posはカプセルのstartとして使おう
	if (m_LR == LeftOrRight::kLeft)
	{
		m_pos = parent->GetArmLeftStartPos();
		m_endPos = parent->GetArmLeftEndPos();
	}
	else
	{
		m_pos = parent->GetArmRightStartPos();
		m_endPos = parent->GetArmRightEndPos();
	}
}
