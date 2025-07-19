#include "EnemyEliteBiteCol.h"
#include <string>
#include "EnemyElite.h"
#include "SphereCollider.h"
#include <DxLib.h>
#include "Rigid.h"
#include "Collidable.h"

namespace
{
	const std::string kTrajectoryEffectName = ".efkefc";
	constexpr float kRadius = 50.0f;
	constexpr float kLifeTime = 20.0f;
	constexpr float kLength = 10.0f;
	constexpr float kAttackPower = 100.0f;
	constexpr float kKnockbackPower = 30.0f;
	constexpr float kDrag = 0.0f;
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
	m_collidable->GetCol().Draw();
}

void EnemyEliteBiteCol::CommitMove()
{
}
