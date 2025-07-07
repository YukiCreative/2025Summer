#include "EnemyTest.h"
#include "Collidable.h"
#include "Rigid.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "AnimationModel.h"
#include <DxLib.h>
#include "Geometry.h"
#include "PlayerSword.h"
#include "AttackCol.h"

namespace
{
	const PhysicalMaterial kPhysiMat =
	{
		0.1f, 0.05f, 0.1f
	};

	// モデル
	const std::string kModelName = "Data/Model/bug.mv1";
	constexpr float kAnimSpeed = 30.0f;
	const std::string kInitAnimName = "Armature|Idle";

	// コライダー
	constexpr float kSphereRadius = 60;
	constexpr float kWeight = 100;

	const Vector3 kColOffset = {0,50,0};
}

EnemyTest::EnemyTest() :
	Actor(true) // ロックオンできるか…なんだけど、ぱっと見じゃ分かりにくい
{
}

void EnemyTest::Init(const Vector3& initPos)
{
	m_kind = ActorKind::kEnemy;

	m_pos = initPos;

	auto col = std::make_shared<SphereCollider>();
	col->Init(m_pos, kWeight, false, false, kSphereRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysiMat);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_model = std::make_shared<AnimationModel>();
	m_model->Init(kModelName, kAnimSpeed);
	m_model->ChangeAnimation(kInitAnimName);
}

void EnemyTest::Update()
{
	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	m_model->Update();

	m_model->SetPos(m_pos);
}

void EnemyTest::Draw() const
{
	m_collidable->GetCol().Draw();

	m_model->Draw();

	DrawSphere3D(m_pos, 10, 10, 0xffffff, 0xffffff, true);
}

void EnemyTest::CommitMove()
{
	const Vector3 vel = m_collidable->UpdateRigid();
	m_pos += vel;

	m_collidable->SetPos(m_pos + kColOffset);
}

void EnemyTest::OnCollision(std::shared_ptr<Actor> other)
{
	if (other->GetKind() == ActorKind::kPlayerAttack)
	{
		auto attack = std::static_pointer_cast<AttackCol>(other);

		// ダメージ
		printf("食らった！%fダメージ！\n", attack->GetAttackPower());

		return;
	}
}
