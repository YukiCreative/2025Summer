#include "Sphere.h"
#include <DxLib.h>

void Sphere::Init(const float radius, const int divNum, const Color::ColorCode_t difColor, const Color::ColorCode_t spcColor, bool isFill)
{
	// コンストラクタはクソ　という理念のもと動いています
	m_radius   = radius;
	m_divNum   = divNum;
	m_difColor = difColor;
	m_spcColor = spcColor;
	m_isFill   = isFill;
}

void Sphere::Draw(const Vector3& pos) const
{
	DrawSphere3D(pos, m_radius, m_divNum, m_difColor, m_spcColor, m_isFill);
}