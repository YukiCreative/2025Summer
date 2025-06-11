#pragma once

// これプロトタイプ宣言です
namespace DxLib
{
	struct tagVECTOR;
};

struct Vector2;

struct Vector3
{
public:
	float x, y, z;
public:
	Vector3();
	Vector3(const float _x, const float _y, const float _z);

	// DxLibのVECTORクラスに変換できるようにする
	operator DxLib::tagVECTOR();
	operator DxLib::tagVECTOR() const;
	Vector3(const DxLib::tagVECTOR&);

	void    operator+=(const Vector3& other);
	Vector3 operator+ (const Vector3& other) const;
	void    operator-=(const Vector3& other);
	Vector3 operator- (const Vector3& other) const;
	void    operator*=(const float other);
	Vector3 operator* (const float other) const;
	void    operator/=(const float other);
	Vector3 operator/ (const float other) const;
	Vector3 operator- () const;
	bool    operator==(const Vector3& other) const;

	// {1,0,0}
	static const Vector3 Right();
	// {-1,0,0}
	static const Vector3 Left();
	// {0,1,0}
	static const Vector3 Up();
	// {0,-1,0}
	static const Vector3 Down();
	// {0,0,1}
	static const Vector3 Foward();
	// {0,0,-1}
	static const Vector3 Back();

	// 正規化したコピーを返す
	Vector3 GetNormalize() const;
	// 自分を正規化
	void  Normaize();
	float SqrMagnitude() const;
	float Magnitude() const;
	float Dot(const Vector3& other) const;
	Vector3 Cross(const Vector3& other) const;
	// 自分が基点
	Vector3 Lerp(const Vector3& targetPos, const float time) const;
	void LerpMyself(const Vector3& targetPos, const float time);
	// Y成分を抜かして返す
	Vector3 XZ() const;
	static Vector3 Zero();
};

