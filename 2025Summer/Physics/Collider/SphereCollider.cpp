#include "Color.h"
#include "SphereCollider.h"
#include <DxLib.h>

namespace
{
	// DrawSphere3Dに使用する定数を定義
	const int kDivNum = 16;
	const Color::ColorCode_t kDifColor = Color::Red;
	const Color::ColorCode_t kSpcColor = Color::White;
	const bool kIsFill = false;
}

SphereCollider::SphereCollider() :
	m_radius(0)
{
	// colKindを上書き
	m_colKind = ColKind3D::kSphere;
}

void SphereCollider::Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic, const float radius)
{
	Collider3D::Init(initPos, weight, isThrough, isStatic);
	m_radius = radius;
}

void SphereCollider::Draw() const
{
	// 当たり判定の有効範囲をフレームで描画
	DrawSphere3D(m_pos, m_radius, kDivNum, kDifColor, kSpcColor, kIsFill);
}