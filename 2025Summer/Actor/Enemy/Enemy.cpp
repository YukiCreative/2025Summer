#include "Enemy.h"
#include "../../Model/AnimationModel.h"
#include "../../Physics/Collidable.h"
#include "../Player/Player.h"
#include "../Player/PlayerAttackCol.h"
#include "../../Geometry/Geometry.h"
#include <DxLib.h>
#include "../../Shader/ShaderDraw.h"
#include "../../Physics/Collider/Collider3D.h"
#include "../../Effect/EffectManager.h"
#include "../../Effect/EffekseerEffect.h"
#include "../Player/PlayerSword.h"
#include "../../Physics/Rigid.h"

namespace
{
	constexpr float kRotateSpeed = 0.1f;
	constexpr float kAnimSpeed = 30.0f;
	constexpr float kDissolveSpeed = 0.02f;
	const std::string kVS = "Data/Shader/EnemyDissolveVS.vso";
	const std::string kPS = "Data/Shader/EnemyWhiteFade.pso";
	const std::string kTex = "Data/Image/pattern.png";

	const std::string kDeathEffect = "BigBlood.efkefc";

	constexpr float kKnockUpStartDrag = 0.1f;
	constexpr float kKnockUpDecreaseDragAmount = 0.001f;

	constexpr int kDamageStopFrame = 2;
}

Enemy::Enemy(const float maxStunPoint, const float maxHitPoint) :
	Actor(true), // 敵はロックオン可能
	m_isInvincible(false),
	m_enemyKind(EnemyKind::kNone),
	m_bloodFrameIndex(0),
	m_stunPoint(maxStunPoint),
	m_hitPoint(maxHitPoint),
	m_isKnockUp(false),
	m_fallFrame(0),
	m_isDamageInThisFrame(false)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(std::weak_ptr<Player> player, const Vector3& initPos, const int dupulicatedHandle)
{
	m_kind = ActorKind::kEnemy;
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

	// フラグリセット
	m_isDamageInThisFrame = false;
}

void Enemy::KnockUpUpdate()
{
	Rigid& rigid = m_collidable->GetRigid();
	Vector2 drag = rigid.GetDrag();
	drag.y -= kKnockUpDecreaseDragAmount;
	rigid.SetDrag(drag);
	if (rigid.IsMinDefaultDragY())
	{
		rigid.SetDragDefault();
	}

	bool shouldSlowFallSpeed = false;
	shouldSlowFallSpeed = rigid.GetVel().y < 0;

	// 落下を開始したとき、一定時間重力を弱める
	if (shouldSlowFallSpeed)
	{
		rigid.SetGravityMagnification(0.02f);
		++m_fallFrame;
	}
	if (m_fallFrame > 30)
	{
		rigid.SetGravityMagnification(1.0f);
	}

	// 落下を始めて着地するまで継続
	if (m_fallFrame && m_isGround)
	{
		rigid.SetGravityMagnification(1.0f);
		m_isKnockUp = false;
		m_fallFrame = 0;
	}
}

void Enemy::Draw() const
{
	m_model->Draw();

#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
}

void Enemy::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	m_isDamageInThisFrame |= other->GetKind() == ActorKind::kPlayerAttack;
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

void Enemy::KnockBack(const Vector3& power)
{
	Vector3 temp = power;

	if (!m_stunPoint.IsStun())
	{
		temp.y = 0;
	}

	m_collidable->AddVel(temp);
}

void Enemy::DecreaseStunPoint(const float point)
{
}

void Enemy::OnStun()
{
	m_collidable->GetRigid().SetDrag({ m_collidable->GetRigid().GetDrag().x, kKnockUpStartDrag });
}

void Enemy::OnDamage(std::weak_ptr<PlayerAttackCol> attack)
{
	// 無敵なら食らわない
	if (m_isInvincible) return;

	auto attackPower = attack.lock()->GetAttackPower();

	// ヒットストップ
	SetStopFrame(kDamageStopFrame);

	m_stunPoint.DecreasePoint(attack.lock()->GetStunPower());
	if (m_stunPoint.IsStun())
	{
		OnStun();
	}

#if _DEBUG
	printf("食らった！%fダメージ！\n", attackPower);
#endif

	m_hitPoint.DecreasePoint(attackPower);

	// プレイヤーの攻撃が打ち上げ属性で、自分がスタンしていたらKnockUp
	m_isKnockUp |= attack.lock()->IsKnockUpAttack() && IsStun();

	// ダメージ状態、死亡状態はそれぞれのクラスによって実装が違うのでここには書けなかった
	// それぞれのOnDamage内でEnemy::OnDamageを呼んでください
}

void Enemy::RecoveryStun()
{
	m_stunPoint.SetMax();
}

void Enemy::SetAnimSpeed(const float speed)
{
	if (!this) return;

	m_model->SetAnimPlaySpeed(speed);
}

bool Enemy::CompareAnim(const std::string& animName) const
{
	return m_model->CheckAnimName(animName);
}
