#include "EnemyBug.h"
#include "Collidable.h"
#include "Rigid.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "AnimationModel.h"
#include <DxLib.h>
#include "AttackCol.h"
#include "EnemyBugState.h"
#include "EnemyBugIdle.h"
#include "EnemyBugDamage.h"
#include "Player.h"
#include "EnemyBugAttackCol.h"

namespace
{
	const PhysicalMaterial kPhysiMat =
	{
		0.1f, 0.05f, 0.1f
	};

	// モデル
	const std::string kModelName = "Data/Model/bug.mv1";
	constexpr float kAnimSpeed = 30.0f;

	// コライダー
	constexpr float kSphereRadius = 60.0f;
	constexpr int kWeight = 100;

	const Vector3 kColOffset = {0,60,0};

	// 攻撃判定を出現させる時に基準にするリグ
	const std::string kCollisionFrameName1 = "bug_mandible_R";
	const std::string kCollisionFrameName2 = "bug_mandible_L";
}

EnemyBug::EnemyBug() :
	Enemy()
{
}

void EnemyBug::Init(std::weak_ptr<Player> player, const Vector3& initPos)
{
	Enemy::Init(player, initPos);

	auto col = std::make_shared<SphereCollider>();
	col->Init(m_pos, kWeight, false, false, kSphereRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysiMat);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_model = std::make_shared<AnimationModel>();
	m_model->Init(kModelName, kAnimSpeed);

	m_state = std::make_shared<EnemyBugIdle>(weak_from_this());
}

void EnemyBug::Update()
{
	m_state = m_state->Update();

	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	// アニメーション進める
	m_model->Update();
}

void EnemyBug::Draw() const
{
	m_model->Draw();

#if _DEBUG
	m_collidable->GetCol().Draw();
#endif


}

void EnemyBug::CommitMove()
{
	const Vector3 vel = m_collidable->UpdateRigid();
	m_pos += vel;

	m_model->SetPos(m_pos);
	m_collidable->SetPos(m_pos + kColOffset);
}

void EnemyBug::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	if (other->GetKind() == ActorKind::kPlayerAttack)
	{
		auto attack = std::static_pointer_cast<AttackCol>(other);

		// ダメージ
		OnDamage(attack);
		return;
	}
}

void EnemyBug::OnCollisionStay(std::shared_ptr<Actor> other)
{
	if (other->GetKind() == ActorKind::kPlayerAttack)
	{
		printf("攻撃貫通中\n");
	}
}

void EnemyBug::OnCollisionExit(std::shared_ptr<Actor> other)
{
	if (other->GetKind() == ActorKind::kPlayerAttack)
	{
		printf("攻撃が抜けた\n");
	}
}

MATRIX EnemyBug::GetModelMatrix() const
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

void EnemyBug::AddVel(const Vector3& vel)
{
	m_collidable->AddVel(vel);
}

bool EnemyBug::IsAnimEnd() const
{
	return m_model->IsEnd();
}

void EnemyBug::GenerateAttackCol()
{
	auto col = std::make_shared<EnemyBugAttackCol>();
	col->Init(weak_from_this());

	m_spawnActorList.emplace_back(col);
}

Vector3 EnemyBug::GetAttackRigPos() const
{
	auto ago1 = m_model->GetFramePosition(kCollisionFrameName1);
	auto ago2 = m_model->GetFramePosition(kCollisionFrameName2);

	return (ago1 + ago2) * 0.5f;
}

void EnemyBug::OnDamage(std::shared_ptr<AttackCol> attack)
{
	// 無敵なら食らわない

	printf("食らった！%fダメージ！\n", attack->GetAttackPower());

	m_state = std::make_shared<EnemyBugDamage>(weak_from_this());

	// プレイヤーの位置を見て吹っ飛ぶ
	auto pToEN = (m_pos - m_player.lock()->GetPos()).GetNormalize();
	m_collidable->AddVel(VTransformSR({ 0,0, attack->GetKnockbackPower() }, MGetRotVec2(Vector3::Foward(), pToEN)));
}
