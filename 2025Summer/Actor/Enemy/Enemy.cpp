#include "Enemy.h"
#include "AnimationModel.h"
#include "Collidable.h"
#include "Player.h"
#include <DxLib.h>

namespace
{
	constexpr float kRotateSpeed = 0.1f;
	constexpr float kAnimSpeed = 30.0f;
}

Enemy::Enemy() :
	Actor(true), // 敵はロックオン可能
	m_isInvincible(false)
{
}

void Enemy::Init(std::weak_ptr<Player> player, const Vector3& initPos, const float initHP, const int dupulicatedHandle)
{
	m_kind = ActorKind::kEnemy;
	m_hitPoint = initHP;
	m_player = player;
	m_pos = initPos;
	m_model = std::make_shared<AnimationModel>();
	m_model->Init(dupulicatedHandle, kAnimSpeed);
}

void Enemy::ChangeAnim(const std::string& animName, const bool isLoop)
{
	m_model->ChangeAnimation(animName, isLoop);
}

void Enemy::LookAtPlayer(const float strength)
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

	// 引数で補正
	rot *= strength;

	m_model->RotateUpVecY(rot);
}

Vector3 Enemy::EnemyToPlayer() const
{
	return m_player.lock()->GetPos() - m_pos;
}

Vector3 Enemy::GetDir() const
{
	auto mat = GetModelMatrix();
	return { mat.m[2][0],mat.m[2][2] ,mat.m[2][2] };
}

MATRIX Enemy::GetModelMatrix() const
{
	auto mat = m_model->GetMatrix();

	mat.m[1][0] *= -1;
	mat.m[1][1] *= -1;
	mat.m[1][2] *= -1;

	mat.m[2][0] *= -1;
	mat.m[2][1] *= -1;
	mat.m[2][2] *= -1;

	return mat;
}

bool Enemy::IsEndAnim() const
{
	return m_model->IsEnd();
}

void Enemy::AddVel(const Vector3& vel)
{
	m_collidable->AddVel(vel);
}