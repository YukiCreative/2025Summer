#include "Vector3.h"
#include <DxLib.h>
#include <cmath>
#include "Vector2.h"

Vector3::Vector3() :
	x(0), y(0), z(0)
{
}

Vector3::Vector3(const float _x, const float _y, const float _z) :
	x(_x), y(_y), z(_z)
{
}

Vector3::operator DxLib::tagVECTOR()
{
	return { x, y, z };
}

Vector3::operator DxLib::tagVECTOR() const
{
	return { x, y, z }; // VGet()‚Æ“¯‚¶
}

Vector3::Vector3(const DxLib::tagVECTOR& vector) :
	x(vector.x), y(vector.y), z(vector.z)
{
}

void Vector3::operator+=(const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

Vector3 Vector3::operator+(const Vector3& other) const
{
	return { x + other.x, y + other.y, z + other.z };
}

void Vector3::operator-=(const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return { x - other.x, y - other.y, z - other.z };
}

void Vector3::operator*=(const float other)
{
	x *= other;
	y *= other;
	z *= other;
}

Vector3 Vector3::operator*(const float other) const
{
	return { x * other, y * other, z * other };
}

void Vector3::operator/=(const float other)
{
	x /= other;
	y /= other;
	z /= other;
}

Vector3 Vector3::operator/(const float other) const
{
	return { x / other, y / other, z / other };
}

Vector3 Vector3::operator-() const
{
	return {-x, -y, -z};
}

bool Vector3::operator==(const Vector3& other) const
{
	return x == other.x &&
		   y == other.y &&
		   z == other.z;
}

const Vector3 Vector3::Right()
{
	return {1,0,0};
}

const Vector3 Vector3::Left()
{
	return {-1,0,0};
}

const Vector3 Vector3::Up()
{
	return {0,1,0};
}

const Vector3 Vector3::Down()
{
	return {0,-1,0};
}

const Vector3 Vector3::Foward()
{
	return {0,0,1};
}

const Vector3 Vector3::Back()
{
	return {0,0,-1};
}

Vector3 Vector3::GetNormalize() const
{
	const float mag = Magnitude();

	if (mag == 0)
	{
		return {x, y, z};
	}

	return { x / mag, y / mag, z / mag };
}

void Vector3::Normalize()
{
	const float mag = Magnitude();

	if (mag == 0)
	{
		return;
	}

	x /= mag;
	y /= mag;
	z /= mag;
}

float Vector3::SqrMagnitude() const
{
	return x * x + y * y + z * z;
}

float Vector3::Magnitude() const
{
	return sqrtf(x*x + y*y + z*z);
}

float Vector3::Dot(const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::Cross(const Vector3& other) const
{
	return
	{
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x,
	};
}

Vector3 Vector3::Lerp(const Vector3& targetPos, const float time) const
{
	Vector3 lerped;
	lerped.x += (targetPos.x - x) * time;
	lerped.y += (targetPos.y - y) * time;
	lerped.z += (targetPos.z - z) * time;
	return lerped;
}

void Vector3::LerpMyself(const Vector3& targetPos, const float time)
{
	x += (targetPos.x - x) * time;
	y += (targetPos.y - y) * time;
	z += (targetPos.z - z) * time;
}

Vector3 Vector3::XZ() const
{
	return {x, 0, z};
}

Vector3 Vector3::Zero()
{
	return {0,0,0};
}
