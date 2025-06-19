#pragma once
#include "Collider3D.h"
class CapsuleCollider : public Collider3D
{
public:
	CapsuleCollider();

	void Init(const Vector3& start, const Vector3 end, const int weight, const bool isThrough, const bool isStatic, const float radius);

	void Draw() const override;

	void SetPos(const Vector3& pos) override;
	void SetPos(const Vector3& start, const Vector3& end);

	const Vector3 StartPos() const;
	const Vector3 EndPos() const;

private:

	Vector3 m_start;
	Vector3 m_end;
	float m_radius;

private:
	Vector3 MiddlePoint() const;
};

