#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include <DxLib.h>

namespace Geometry
{
	constexpr float kPi = 3.14159265f;
	constexpr float kRadToDeg = 180 / kPi;
	constexpr float kDegToRad = kPi / 180;

	// movePosがbasePosを回転の中心にY軸方向にrad回転した位置を返す
	Vector3 GetRotatedPosUpVecY(const Vector3& movePos, const Vector3& basePos, const float rad);

	// GetRotatedPosUpVecYの関数の行列を返すVer
	MATRIX GetRotateMatrixUpVecY(const Vector3& movePos, const Vector3& basePos, const float rad);

	// GetRotatedPosUpVecYの任意回転軸Ver
	Vector3 GetRotatedPosAnyUpVec(const Vector3& movePos, const Vector3& basePos, const Vector3& axis, const float rad);

	// 二つの3次元ベクトルのなす角(度数)
	float Corner(const Vector3& a, const Vector3& b);
}