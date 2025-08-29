#pragma once
#include <algorithm>
#include "../Geometry/Geometry.h"

// �̗͒l
class HitPoint
{
public:
	HitPoint(const float maxPoint) : m_point(maxPoint), m_maxPoint(maxPoint) {}

	void DecreasePoint(const float right)
	{
		m_point -= right;
		m_point = std::max(m_point, 0.0f);
	}

	void SetMax()
	{
		m_point = m_maxPoint;
	}

	bool IsMin() const
	{
		return m_point <= Geometry::kEpsilon;
	}

	float Value() const { return m_point; }

	float GetRatio() const
	{
		return m_point / m_maxPoint;
	}

private:

	float m_point;
	float m_maxPoint;
};

