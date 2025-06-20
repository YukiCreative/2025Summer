#include "EnemyTest.h"
#include "Collidable.h"
#include "Rigid.h"
#include "SphereCollider.h"
#include "AnimationModel.h"

namespace
{
	const PhysicalMaterial kPhysiMat =
	{
		0.1f, 0.05f, 0.1f
	};
}

EnemyTest::EnemyTest()
{
}

void EnemyTest::Init()
{
	m_pos = { 200, 0, 0 };

	auto col = std::make_shared<SphereCollider>();
	col->Init(m_pos, 10, false, false, 30);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysiMat);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_model = std::make_shared<AnimationModel>();
	m_model->Init(-1, 1);
}

void EnemyTest::Update()
{
	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}
}

void EnemyTest::Draw() const
{
	m_collidable->GetCol().Draw();

	m_model->Draw();
}

void EnemyTest::CommitMove()
{
	const Vector3 vel = m_collidable->UpdateRigid();
	m_pos += vel;

	m_collidable->SetPos(m_pos);
}
