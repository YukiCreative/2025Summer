#include "Collider3D.h"

Collider3D::Collider3D() :
	m_colKind(ColKind3D::kNone), // Noneはエラーを吐くための属性
	m_pos(),
	m_isMove(false),
	m_isStatic(false),
	m_weight(1),
	m_isThrough(false)
{
}

Collider3D::~Collider3D()
{
}

void Collider3D::Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic)
{
	m_pos = initPos;
	m_weight = weight;
	m_isThrough = isThrough;
	m_isStatic = isStatic;
}

void Collider3D::SetPos(const Vector3& pos)
{
	// 位置が変わってれば動いている
	// すでに今フレームで動いているときはしっかりtrueを維持
	m_isMove = m_isMove || m_pos != pos;
	m_pos = pos;
}