#include "Geometry.h"
#include <cmath>
Vector3 Geometry::GetRotatedPosUpVecY(const Vector3& movePos, const Vector3& basePos, const float rad)
{
	// basePosが原点だった時のmovePosの位置を出す
	const Vector3 movedPos = movePos - basePos;
	// 出した位置を、原点を中心に回転させる
	const Vector3 rotatedMovedPos = VTransform(movedPos, MGetRotY(rad));
	// targetPosを原点に戻す
	const Vector3 movedRotatedMovedPos = rotatedMovedPos + basePos;
	// これを返す
	return movedRotatedMovedPos;
}

MATRIX Geometry::GetRotateMatrixUpVecY(const Vector3& movePos, const Vector3& basePos, const float rad)
{
	// ずらす
	const MATRIX moveMat = MGetTranslate(-basePos);
	// 回す
	const MATRIX rotateMat = MGetRotY(rad);
	const MATRIX rotatedMovedMat = MMult(moveMat, rotateMat);
	// 逆に戻していく
	const MATRIX returnMat = MGetTranslate(basePos);
	const MATRIX movedRotatedMovedMat = MMult(rotatedMovedMat, returnMat);

	return movedRotatedMovedMat;
}

Vector3 Geometry::GetRotatedPosAnyUpVec(const Vector3& movePos, const Vector3& basePos, const Vector3& axis, const float rad)
{
	// basePosが原点だった時のmovePosの位置を出す
	const Vector3 movedPos = movePos - basePos;
	// 出した位置を、原点を中心に回転させる
	const Vector3 rotatedMovedPos = VTransform(movedPos, MGetRotY(rad));
	// targetPosを原点に戻す
	const Vector3 movedRotatedMovedPos = rotatedMovedPos + basePos;
	// これを返す
	return movedRotatedMovedPos;
}

float Geometry::Corner(const Vector3& a, const Vector3& b)
{
	const float aMag = a.Magnitude();
	const float bMag = b.Magnitude();

	if (aMag < 0.001f || bMag < 0.001f)
	{
		return 0;
	}

	// 公式通りにやるとだいぶ重たそうに見える処理になったな
	float deg = acosf(a.Dot(b) / (aMag * bMag));

	return deg;
}