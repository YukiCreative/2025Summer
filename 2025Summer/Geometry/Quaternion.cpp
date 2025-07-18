#include "Quaternion.h"
#include <cmath>
#include <DxLib.h>

Quaternion::Quaternion()
{
	*this = Identity();
}

Quaternion::Quaternion(const float _w, const Vector3& axis)
{
	w = _w;
	x = axis.x;
	y = axis.y;
	z = axis.z;
}

Quaternion::Quaternion(const float _w, const float _x, const float _y, const float _z)
{
	w = _w;
	x = _x;
	y = _y;
	z = _z;
}

Quaternion Quaternion::Identity()
{
	// Unityはこうなってた
	return Quaternion(1,0,0,0);
}

Quaternion Quaternion::AngleAxis(const float angle, const Vector3& axis)
{
	// 一応正規化
	auto axisN = axis.GetNormalize();

	Quaternion result = Identity();

	result.w = cosf(angle / 2.0f);
	result.x = axisN.x * sinf(angle / 2.0f);
	result.y = axisN.y * sinf(angle / 2.0f);
	result.z = axisN.z * sinf(angle / 2.0f);

	return result;
}

Quaternion Quaternion::P2Q(const Vector3& pos)
{
	return Quaternion(1.0f, pos);
}

Quaternion Quaternion::Slerp(const Quaternion& start, const Quaternion& end, const float time)
{
	float theta = acosf(Quaternion::Dot(start,end));
	float sinTheta = sinf(theta);

	return start * (sinf((1 - time) * theta) / sinTheta) + 
		   end   * (sinf(time * theta)       / sinTheta);
}

float Quaternion::Magnitude() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

float Quaternion::SqrMagnitude() const 
{
	return x * x + y * y + z * z + w * w;
}

float Quaternion::Dot(const Quaternion& other) const 
{
	return w * other.w + x * other.x + y * other.y + z * other.z;
}

float Quaternion::Dot(const Quaternion& left, const Quaternion& right)
{
	return left.w * right.w + left.x * right.x + left.y * right.y + left.z * right.z;
}

Quaternion Quaternion::GetNormalize() const 
{
	return *this / Magnitude();
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	Quaternion result = Identity();

	// 普通に計算法則に則って計算
	// 実数,i,j,kの項にまとめてそれぞれ代入
	result.w = w * other.w - x * other.x - y * other.y - z * other.z;
	result.x = w * other.x + x * other.w + y * other.z - z * other.y;
	result.y = w * other.y + y * other.w + z * other.x - x * other.z;
	result.z = w * other.z + z * other.w + x * other.y - y * other.x;

	return result;
}

Quaternion Quaternion::operator+(const Quaternion& other) const
{
	return {w + other.w, x + other.x, y + other.y, z + other.z};
}

Vector3 Quaternion::operator*(const Vector3& pos) const
{
	Vector3 result;
	// 掛け算したいのでクォータニオンにする
	Quaternion posQ = P2Q(pos);
	Quaternion temp = Identity();

	// かけた結果
	temp = *this * posQ * this->Inverce();

	//３次元座標に戻す
	result.x = temp.x;
	result.y = temp.y;
	result.z = temp.z;

	return result;
}

Quaternion Quaternion::operator*(const float right) const 
{
	return {w*right,x * right,y * right,z * right};
}

Quaternion Quaternion::operator/(const float right) const
{
	return {w/right,x / right,y / right,z / right};
}

Quaternion Quaternion::Inverce() const
{
	return Quaternion(w, -x, -y, -z);
}

DxLib::tagMATRIX Quaternion::ToMatrix() const
{
	MATRIX result;

	result.m[0][0] = 2 * w * w + 2 * x * x - 1;
	result.m[0][1] = 2 * x * y - 2 * z * w;
	result.m[0][2] = 2 * x * z + 2 * y * w;
	result.m[1][0] = 2 * x * y + 2 * z * w;
	result.m[1][1] = 2 * w * w + 2 * y * y - 1;
	result.m[1][2] = 2 * y * z - 2 * x * w;
	result.m[2][0] = 2 * x * z - 2 * y * w;
	result.m[2][1] = 2 * y * z + 2 * x * w;
	result.m[2][2] = 2 * w * w + 2 * z * z - 1;

	return result;
}