#include "PlayerSpecialAttackCol.h"
#include "Collidable.h"

namespace
{
	constexpr int kLifeTime = 1;
	constexpr float kRadius = 1000.0f;
	constexpr float kKnockbackPower = 50.0f;
	constexpr float kAttackPower = 2000.0f;
	constexpr float kHitStopFrame = 60;
}

PlayerSpecialAttackCol::PlayerSpecialAttackCol() :
	AttackCol(),
	m_frame(0)
{
}

PlayerSpecialAttackCol::~PlayerSpecialAttackCol()
{
}

void PlayerSpecialAttackCol::Init(std::weak_ptr<Player> parent, const Vector3& initPos)
{
	m_attackPower = kAttackPower;
	m_knockbackPower = kKnockbackPower;

	m_pos = initPos;
	m_collidable->SetPos(initPos);

	// TODO:エフェクトの制作と再生
}

void PlayerSpecialAttackCol::Update()
{
	if (m_frame > kLifeTime)
	{
		Destroy();
	}

	++m_frame;
}

void PlayerSpecialAttackCol::Draw() const
{
}

void PlayerSpecialAttackCol::CommitMove()
{
}
