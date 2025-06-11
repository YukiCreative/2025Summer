#include "Quaternion.h"
#include <cmath>

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
	// UnityÇÕÇ±Ç§Ç»Ç¡ÇƒÇΩ
	return Quaternion(1,0,0,0);
}

Quaternion Quaternion::AngleAxis(const float angle, const Vector3& axis)
{
	// àÍâûê≥ãKâª
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

	// ïÅí Ç…åvéZñ@ë•Ç…ë•Ç¡ÇƒåvéZ
	// é¿êî,i,j,kÇÃçÄÇ…Ç‹Ç∆ÇﬂÇƒÇªÇÍÇºÇÍë„ì¸
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
	// ä|ÇØéZÇµÇΩÇ¢ÇÃÇ≈ÉNÉHÅ[É^ÉjÉIÉìÇ…Ç∑ÇÈ
	Quaternion posQ = P2Q(pos);
	Quaternion temp = Identity();

	// Ç©ÇØÇΩåãâ 
	temp = *this * posQ * this->Inverce();

	//ÇRéüå≥ç¿ïWÇ…ñﬂÇ∑
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
