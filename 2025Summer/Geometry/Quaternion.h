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
	// axis������angle���W�A����]�ł���Quaternion��Ԃ��܂�
	static Quaternion AngleAxis(const float angle, const Vector3& axis);
	// ���W���N�H�[�^�j�I����
	static Quaternion P2Q(const Vector3& pos);
	static Quaternion Slerp(const Quaternion& start, const Quaternion& end, const float time);

	float Magnitude() const;
	float SqrMagnitude() const;
	// �l�����x�N�g���Ƃ��Ă̓���
	float Dot(const Quaternion& other) const;
	static float Dot(const Quaternion& left, const Quaternion& right);
	// ���K��
	Quaternion GetNormalize() const;

	// �N�H�[�^�j�I�����m�̊|���Z
	Quaternion operator*(const Quaternion& other) const;
	Quaternion operator+(const Quaternion& other) const;
	// �x�N�g�������W�ϊ�
	Vector3    operator*(const Vector3& pos) const;
	// �����Ƃ̏揜
	Quaternion operator*(const float right) const;
	Quaternion operator/(const float right) const;

	// �t��]
	Quaternion Inverce() const;

public:
	float w, x, y, z;
};