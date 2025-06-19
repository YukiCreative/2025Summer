#include "CapsuleCollider.h"
#include <DxLib.h>

CapsuleCollider::CapsuleCollider() :
	m_radius(0)
{
	m_colKind = ColKind3D::kCapsule;
}

void CapsuleCollider::Init(const Vector3& start, const Vector3 end, const int weight, const bool isThrough, const bool isStatic, const float radius)
{
	m_start = start;
	m_end = end;
	m_radius = radius;

	// 基底側のposは中心座標を入れておく
	Collider3D::Init(MiddlePoint(), weight, isThrough, isStatic);
}

void CapsuleCollider::Draw() const
{
	// カプセルのフレームを描画
	DrawCapsule3D(m_start, m_end, m_radius, 10, 0xff0000, 0xff0000, false);
}

void CapsuleCollider::SetPos(const Vector3& pos)
{
	// 平行移動
	const Vector3 diff = pos - m_pos;

	m_pos    =  pos;
	m_start -= diff;
	m_end   -= diff;
}

void CapsuleCollider::SetPos(const Vector3& start, const Vector3& end)
{
	// startとendを上書き
	m_start = start;
	m_end   = end;
	m_pos   = MiddlePoint();
}

const Vector3 CapsuleCollider::StartPos() const
{
	return m_start;
}

const Vector3 CapsuleCollider::EndPos() const
{
	return m_end;
}

const Vector3 CapsuleCollider::Direction() const
{
	return (m_end - m_start).GetNormalize();
}

Vector3 CapsuleCollider::MiddlePoint() const
{
	return (m_start + m_end) * 0.5f;
}
