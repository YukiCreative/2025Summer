#include "Enemy.h"
#include "AnimationModel.h"
#include "Collidable.h"
#include "Player.h"

namespace
{
	constexpr float kRotateSpeed = 0.1f;
}

Enemy::Enemy() :
	Actor(true) // 敵はロックオン可能
{
}

void Enemy::Init(std::weak_ptr<Player> player, const Vector3& initPos)
{
	m_kind = ActorKind::kEnemy;

	m_player = player;

	m_pos = initPos;
}

void Enemy::ChangeAnim(const std::string& animName)
{
	m_model->ChangeAnimation(animName);
}

void Enemy::LookAtPlayer()
{
	auto eToPXZN = (m_player.lock()->GetPos().XZ() - m_pos.XZ()).GetNormalize();
	auto dir = m_model->GetDirection();
	auto cross = dir.Cross(eToPXZN);

	auto dot = dir.Dot(eToPXZN);

	float rot = cross.y * kRotateSpeed;

	// 正反対
	if (cross.y < 0.001f && dot < -0.9999f)
	{
		rot += 0.1f;
	}

	m_model->RotateUpVecY(rot);
}

Vector3 Enemy::EnemyToPlayer() const
{
	return m_player.lock()->GetPos() - m_pos;
}
