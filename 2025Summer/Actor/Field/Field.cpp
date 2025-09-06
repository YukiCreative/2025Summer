#include "Field.h"
#include "../../Model/Model.h"
#include "../../Physics/Collidable.h"
#include "../../Physics/Collider/MeshCollider.h"
#include "../../Physics/Rigid.h"

namespace
{
	// この辺はステージの当たり判定においては全部同じなので定数で
	constexpr int kWeight = 0;
	constexpr bool kIsThrough = false;
	constexpr bool kIsStatic = true;

	const Vector2 kDrag = { 0.0f, 0.0f };
}

Field::Field() :
	Actor(false)
{
}

void Field::Init(const std::string& modelPath)
{
	m_model = std::make_shared<Model>();
	m_model->Init(modelPath);

	std::shared_ptr<MeshCollider> meshCol = std::make_shared<MeshCollider>();
	meshCol->Init(m_model, kWeight, kIsThrough, kIsStatic);

	std::shared_ptr<Rigid> rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(meshCol, rigid);
}

void Field::Update()
{
}

void Field::Draw() const
{
	m_model->Draw();
}

void Field::CommitMove()
{
}
