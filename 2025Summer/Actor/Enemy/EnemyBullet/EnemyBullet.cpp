#include "EnemyBullet.h"
#include "AnimationModel.h"

namespace
{
	constexpr float kInitHP = 750.0f;
	const std::string kModelName = "Plant.mv1";
}

EnemyBullet::EnemyBullet()
{
}

void EnemyBullet::Init(const std::weak_ptr<Player> player, const Vector3& initPos)
{
	Enemy::Init(player, initPos, kInitHP);

	// ƒ‚ƒfƒ‹
	m_model = std::make_shared<AnimationModel>();
	//m_model->Init(kModelName, );

	// “–‚½‚è”»’è

	// ‰Šúó‘Ô
}

void EnemyBullet::Update()
{
}

void EnemyBullet::Draw() const
{
}

void EnemyBullet::CommitMove()
{
}
