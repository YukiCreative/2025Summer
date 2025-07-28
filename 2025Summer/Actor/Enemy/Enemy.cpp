#include "Enemy.h"
#include "AnimationModel.h"
#include "Collidable.h"
#include "Player.h"
#include "AttackCol.h"
#include "Geometry.h"
#include <DxLib.h>
#include "ShaderDraw.h"
#include "Collider3D.h"
#include "EffectManager.h"
#include "EffekseerEffect.h"

namespace
{
	constexpr float kRotateSpeed = 0.1f;
	constexpr float kAnimSpeed = 30.0f;
	constexpr float kDissolveSpeed = 0.02f;
	const std::string kVS = "Data/Shader/EnemyDissolveVS.vso";
	const std::string kPS = "Data/Shader/EnemyWhiteFade.pso";
	const std::string kTex = "Data/Image/pattern.png";

	const std::string kDeathEffect = "BigBlood.efkefc";
}

Enemy::Enemy() :
	Actor(true), // 敵はロックオン可能
	m_isInvincible(false),
	m_enemyKind(EnemyKind::kNone),
	m_bloodFrameIndex(0)
{
}

Enemy::~Enemy()
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

	m_model->SetPos(m_pos);
}

void Enemy::Update()
{
	UpdateState();

	m_model->Update();

	if (!m_bloodEffect.expired())
	{
		// 派生先で出したいフレームを指定して出す
		m_bloodEffect.lock()->SetPos(m_model->GetFramePosition(m_bloodFrameIndex));
	}
}

void Enemy::Draw() const
{
	m_model->Draw();

#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
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
	if (cross.y < Geometry::kEpsilon && dot < -1.0f + Geometry::kEpsilon)
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

void Enemy::RotateAxisY(const float rad)
{
	m_model->RotateUpVecY(rad);
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

void Enemy::StartBloodEffect()
{
	// 生成
	m_bloodEffect = EffectManager::GetInstance().GenerateEffect(kDeathEffect, m_pos);
}

void Enemy::DisableLockOn()
{
	SetCanLockOn(false);
}