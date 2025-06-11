#include "PolygonCollider.h"
#include <DxLib.h>
#include <string>
#include <cassert>

namespace
{
	// ポリゴンの当たり判定をさせたいときは必ずこの名前のフレームを用意すること。
	const std::string kCollisionFrameName = "CollisionFrame";
}

PolygonCollider::PolygonCollider() :
	Collider3D(),
	m_model(-1),
	m_frameIndex(-1),
	m_useTransform(false)
{
}

PolygonCollider::~PolygonCollider()
{
	// 参照用メッシュの後始末
	MV1TerminateReferenceMesh(m_model, m_frameIndex, m_useTransform);
}

void PolygonCollider::Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic, const int handle, const bool useTransform)
{
	Collider3D::Init(initPos, weight, isThrough, isStatic);
	m_colKind = ColKind3D::kPolygon;
	m_model = handle;
	m_useTransform = useTransform;
	// これはしないといけないらしい
	MV1SetupCollInfo(m_model);

	// 当たり判定用のフレームをセットアップ
	m_frameIndex = MV1SearchFrame(m_model, kCollisionFrameName.c_str());
	assert(m_frameIndex != -1 && "そもそもモデルが読み込めてないかも");
	assert(m_frameIndex != -2 && "指定名のフレームがありません");
	MV1SetupReferenceMesh(m_model, m_frameIndex, m_useTransform);
}

void PolygonCollider::RefreshRefMesh()
{
	// 移動しないなら飛ばそう
	if (!m_useTransform) return;

	MV1RefreshReferenceMesh(m_model, m_frameIndex, m_useTransform);
}

void PolygonCollider::Draw() const
{
	MV1DrawModel(m_model);
}

const DxLib::tagMV1_REF_POLYGONLIST PolygonCollider::GetRefMesh()
{
	return MV1GetReferenceMesh(m_model, m_frameIndex, m_useTransform);
}
