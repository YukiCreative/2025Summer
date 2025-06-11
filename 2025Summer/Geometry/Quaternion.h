#pragma once
#include "Vector3.h"

class Quaternion
{
private:
	Quaternion(const float _w, const Vector3& axis);
	Quaternion(const float _w, const float _x, const float _y, const float _z);

public:
	Quaternion();

	static Quaternion Identity();
	// axisを軸にangleラジアン回転できるQuaternionを返します
	static Quaternion AngleAxis(const float angle, const Vector3& axis);
	// 座標をクォータニオンに
	static Quaternion P2Q(const Vector3& pos);
	static Quaternion Slerp(const Quaternion& start, const Quaternion& end, const float time);

	float Magnitude() const;
	float SqrMagnitude() const;
	// 四次元ベクトルとしての内積
	float Dot(const Quaternion& other) const;
	static float Dot(const Quaternion& left, const Quaternion& right);
	// 正規化
	Quaternion GetNormalize() const;

	// クォータニオン同士の掛け算
	Quaternion operator*(const Quaternion& other) const;
	Quaternion operator+(const Quaternion& other) const;
	// ベクトルを座標変換
	Vector3    operator*(const Vector3& pos) const;
	// 実数との乗除
	Quaternion operator*(const float right) const;
	Quaternion operator/(const float right) const;

	// 逆回転
	Quaternion Inverce() const;

public:
	float w, x, y, z;
};