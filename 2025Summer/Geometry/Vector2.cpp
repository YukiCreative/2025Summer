#include "Geometry.h"
#include "Vector2.h"
#include <cmath>

Vector2::Vector2() :
	x(0), y(0)
{
}

Vector2::Vector2(const float _x, const float _y) :
	x(_x), y(_y)
{
}

Vector2::operator Vector3()
{
	return { x, y, 0 };
}

void Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return { x + other.x, y + other.y };
}

void Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return { x - other.x, y - other.y };
}

void Vector2::operator*=(const float other)
{
	x *= other;
	y *= other;
}

Vector2 Vector2::operator*(const float other) const
{
	return { x * other, y * other };
}

void Vector2::operator/=(const float other)
{
	x /= other;
	y /= other;
}

Vector2 Vector2::operator/(const float other) const
{
	return { x / other, y / other };
}

const Vector2 Vector2::Zero()
{
	return {0,0};
}

const Vector2 Vector2::Up()
{
	return {0,1};
}

const Vector2 Vector2::Down()
{
	return {0,-1};
}

const Vector2 Vector2::Right()
{
	return {1,0};
}

const Vector2 Vector2::Left()
{
	return {-1,0};
}

Vector2 Vector2::GetNormalize() const
{
	const float mag = Magnitude();
	return { x / mag, y / mag };
}

void Vector2::Normalize()
{
	const float mag = Magnitude();
	x /= mag;
	y /= mag;
}

float Vector2::SqrMagnitude() const
{
	return x * x + y * y;
}

float Vector2::Magnitude() const
{
	return sqrtf(x * x + y * y);
}

float Vector2::Dot(const Vector2& other) const
{
	return x * other.x + y * other.y;
}

Vector2 Vector2::Lerp(const Vector2& targetPos, const float time) const
{
	Vector2 lerped;
	lerped.x += (targetPos.x - x) * time;
	lerped.y += (targetPos.y - y) * time;
	return lerped;
}

void Vector2::LerpMyself(const Vector2& targetPos, const float time)
{
	x += (targetPos.x - x) * time;
	y += (targetPos.y - y) * time;
}

float Vector2::Angle() const
{
	float rad = atan2(y, x);
	if (rad < 0)
	{
		rad += Geometry::kPi * 2;
	}
	return rad * Geometry::kRadToDeg;
}

float Vector2::RerativeAngle(const Vector2& other) const
{
	float myDeg = this->Angle();
	float otherDeg = other.Angle();
	return myDeg - otherDeg;
}