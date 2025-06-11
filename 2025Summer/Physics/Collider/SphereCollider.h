#pragma once
#include "Collider3D.h"

class SphereCollider : public Collider3D
{
public:
	SphereCollider();

	void Init(const Vector3& initPos, const int weight, const bool isThrough, const bool isStatic, const float radius);

	void Draw() const override;

	void SetRadius(const float radius) { m_radius = radius; }
	const float GetRadius() const { return m_radius; }
private:
	float m_radius;
};

