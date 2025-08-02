#pragma once
#include "../Geometry/Vector3.h"
#include "../General/Color.h"

// DrawSphere3Dをクラス化した
class Sphere
{
private:
	float m_radius;
	int m_divNum;
	Color::ColorCode_t m_difColor;
	Color::ColorCode_t m_spcColor;
	bool m_isFill;
public:
	void Init(const float radius, const int divNum, const Color::ColorCode_t difColor, const Color::ColorCode_t spcColor, bool fillFlag);

	// 座標だけはこのクラスを持っているクラスから貰う
	void Draw(const Vector3& pos) const;

	void SetDifColor(const Color::ColorCode_t difColor) { m_difColor = difColor; }
};