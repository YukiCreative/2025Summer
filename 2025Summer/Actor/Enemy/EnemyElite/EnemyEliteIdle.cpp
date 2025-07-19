#include "EnemyEliteIdle.h"
#include "EnemyElite.h"
#include "EnemyEliteBite.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
	constexpr float kRotateSpeed = 0.01f;
}

EnemyEliteIdle::EnemyEliteIdle(std::weak_ptr<EnemyElite> parent) :
	EnemyEliteState(parent)
{
	m_parent.lock()->ChangeAnim(kAnimName);
	m_attackFrame = m_parent.lock()->GetAttackInterval();
}

EnemyEliteIdle::~EnemyEliteIdle()
{
}

std::shared_ptr<EnemyEliteState> EnemyEliteIdle::Update()
{
	auto parent = m_parent.lock();

	// ランダムな間隔で攻撃
	if (m_frame > m_attackFrame)
	{
		return std::make_shared<EnemyEliteBite>(m_parent);
	}

	// プレイヤーのほうを向く
	// ちょっと速度にムラをつける
	parent->LookAtPlayer(fabsf(sinf(m_frame * kRotateSpeed)) * 0.5f);

	++m_frame;

	return shared_from_this();
}
