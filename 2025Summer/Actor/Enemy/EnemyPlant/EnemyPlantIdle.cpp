#include "EnemyPlantIdle.h"
#include "EnemyPlant.h"
#include "EnemyPlantAttack.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
	constexpr float kRotateSpeed = 0.02f;
	constexpr float kEscapeDistance = 500.0f;
	constexpr float kEscapeSpeed = 0.2f;
}

EnemyPlantIdle::EnemyPlantIdle(std::weak_ptr<EnemyPlant> parent) :
	EnemyPlantState(parent)
{
	// Anim
	m_parent.lock()->ChangeAnim(kAnimName);

	// 今回の攻撃フレームを取得
	m_attackFrame = m_parent.lock()->GetAttackInterval();
}

EnemyPlantIdle::~EnemyPlantIdle()
{
}

std::shared_ptr<EnemyPlantState> EnemyPlantIdle::Update()
{
	auto parent = m_parent.lock();

	// プレイヤーのほうを向く
	// ちょっと速度にムラをつける
	parent->LookAtPlayer(fabsf(sinf(m_frame * kRotateSpeed)) * 0.5f + 0.1f);

	// プレイヤーが一定距離に近づいたら離れる方向に移動
	if (parent->EnemyToPlayer().SqrMagnitude() < kEscapeDistance * kEscapeDistance)
	{
		parent->AddVel(-parent->EnemyToPlayer().GetNormalize() * kEscapeSpeed);
	}

	// ランダムな間隔で攻撃
	if (m_frame == m_attackFrame)
	{
		return std::make_shared<EnemyPlantAttack>(m_parent);
	}

	++m_frame;

	return shared_from_this();
}
