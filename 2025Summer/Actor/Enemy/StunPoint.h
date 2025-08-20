#pragma once
#include <algorithm>

// ìGÇÃãCê‚íl
class StunPoint
{
public:
	StunPoint(const float maxPoint) : m_point(maxPoint), m_maxPoint(maxPoint) {}

	void DecreasePoint(const float right)
	{
		m_point -= right;
		m_point = std::max(m_point, 0.0f);
	}

	void SetMax()
	{
		m_point = m_maxPoint;
	}

	bool IsStun() const
	{
		return m_point <= 0.0f;
	}

private:

	float m_point;
	float m_maxPoint;
};

