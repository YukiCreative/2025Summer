#include "Enemy.h"
#include "AnimationModel.h"
#include "Collidable.h"
#include "Player.h"
#include "AttackCol.h"
#include "Geometry.h"
#include <DxLib.h>
#include "ShaderDraw.h"
#include "Collider3D.h"

namespace
{
	constexpr float kRotateSpeed = 0.1f;
	constexpr float kAnimSpeed = 30.0f;
	constexpr float kDissolveSpeed = 0.02f;
	const std::string kVS = "Data/Shader/EnemyDissolveVS.vso";
	const std::string kPS = "Data/Shader/EnemyWhiteFade.pso";
	const std::string kTex = "Data/Image/pattern.png";
}

Enemy::Enemy() :
	Actor(true), // 敵はロックオン可能
	m_isInvincible(false),
	m_isDissolving(false),
	m_cBuffH(-1),
	m_cBuff(nullptr),
	m_dissolveTex(-1),
	m_dissolvePsH(-1),
	m_dissolveVsH(-1)
{
}

Enemy::~Enemy()
{
	DeleteShaderConstantBuffer(m_cBuffH);
	DeleteShader(m_dissolvePsH);
	DeleteShader(m_dissolveVsH);
	DeleteGraph(m_dissolveTex);
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

	InitDissolve();
}

void Enemy::Update()
{
	UpdateState();

	m_model->Update();

	if (m_isDissolving)
	{
		UpdateDissolve();
	}
}

void Enemy::Draw() const
{
	if (m_isDissolving)
	{
		DissolveDraw();
	}
	else
	{
		m_model->Draw();
	}

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

void Enemy::UpdateDissolve()
{
	m_cBuff->time += kDissolveSpeed;
	if (m_cBuff->time >= 1.0f) m_cBuff->time = 1.0f;
	UpdateShaderConstantBuffer(m_cBuffH);
}

void Enemy::DissolveDraw() const
{
	SetShaderConstantBuffer(m_cBuffH, DX_SHADERTYPE_PIXEL, 4);
	ShaderDraw::DrawModel(m_model, m_dissolvePsH, m_dissolveVsH, m_dissolveTex);
}

void Enemy::InitDissolve()
{
	m_cBuffH = CreateShaderConstantBuffer(sizeof(EnemyCBuff));
	m_cBuff = (EnemyCBuff*)GetBufferShaderConstantBuffer(m_cBuffH);
	m_cBuff->time = 0.0f;
	UpdateShaderConstantBuffer(m_cBuffH);

	m_dissolvePsH = LoadPixelShader(kPS.c_str());
	m_dissolveVsH = LoadVertexShader(kVS.c_str());
	m_dissolveTex = LoadGraph(kTex.c_str());
}
