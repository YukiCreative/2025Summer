#include "EnemyBugAttackCol.h"
#include "EnemyBug.h"
#include "Collidable.h"
#include "EffectManager.h"
#include "EffekseerEffect.h";
#include "SphereCollider.h"
#include "Rigid.h"

namespace
{
	const std::string kEffectName = "SwordTrajectory.efkefc";
	constexpr float kRadius = 50.0f;
	constexpr float kLifeTime = 20.0f;
}

EnemyBugAttackCol::EnemyBugAttackCol() :
	m_frame(0)
{
}

EnemyBugAttackCol::~EnemyBugAttackCol()
{
}

void EnemyBugAttackCol::Init(std::weak_ptr<EnemyBug> parent)
{
	m_parent = parent;

	m_pos = m_parent.lock()->GetAttackRigPos();

	auto col = std::make_shared<SphereCollider>();
	col->Init(m_pos, 0, true, true, kRadius);

	auto rigid = std::make_shared<Rigid>();
	rigid->Init({0,0,0});

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	// エフェクト生成
	m_effect = EffectManager::GetInstance().GenerateEffect(kEffectName, m_pos);
}

void EnemyBugAttackCol::Update()
{
	// 特定のフレームに追従する
	m_pos = m_parent.lock()->GetAttackRigPos();
	m_collidable->SetPos(m_pos);

	// エフェクトも
	if (!m_effect.expired())
	{
		m_effect.lock()->SetPos(m_pos);
	}

	if (m_frame > kLifeTime)
	{
		m_isAlive = false;
	}

	++m_frame;
}

void EnemyBugAttackCol::Draw() const
{
#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
}

void EnemyBugAttackCol::CommitMove()
{
}
