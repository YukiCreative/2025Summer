#include "MeshCollider.h"
#include <DxLib.h>
#include <string>
#include <cassert>
#include "../../Model/Model.h"

namespace
{
	// ポリゴンの当たり判定をさせたいときは必ずこの名前のフレームを用意すること。
	const std::string kCollisionFrameName = "CollisionFrame";
}

MeshCollider::MeshCollider() :
	Collider3D(),
	m_collisionFrameIndex(-2)
{
}

MeshCollider::~MeshCollider()
{
}

void MeshCollider::Init(std::shared_ptr<Model> model, const int weight, const bool isThrough, const bool isStatic)
{
	Collider3D::Init(model->GetPos(), weight, isThrough, isStatic);
	m_colKind = ColKind3D::kPolygon;
	m_model = model;
	// 当たり判定の準備
	MV1SetupCollInfo(m_model->m_handle);

	// 当たり判定用のフレームを確認、記憶しておく
	// Serchという名前から比較的処理負荷のかかる関数と推測
	m_collisionFrameIndex = MV1SearchFrame(m_model->m_handle, kCollisionFrameName.c_str());
	assert(m_collisionFrameIndex != -2 && "メッシュの当たり判定はCollisionFrameというフレームが必要です");
}

void MeshCollider::Draw() const
{
	MV1DrawFrame(m_model->m_handle, m_collisionFrameIndex);
}

void MeshCollider::UpdateCol() const
{
	MV1RefreshCollInfo(m_model->m_handle, m_collisionFrameIndex);
}

int MeshCollider::GetModelHadle() const
{
	return m_model->m_handle;
}

int MeshCollider::GetCollisionFrameIndex() const
{
	return m_collisionFrameIndex;
}
