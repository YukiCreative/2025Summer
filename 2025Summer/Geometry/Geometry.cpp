#include "Geometry.h"
#include <cmath>
#include <algorithm>
#include <cassert>

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

	// floatの誤差を収める
	auto cosign = a.Dot(b) / (aMag * bMag);
	cosign = std::clamp(cosign, -1.0f, 1.0f);

	float rad = acosf(cosign);

	return rad;
}

Vector3 Geometry::PointSegmentNearestPos(const Vector3& point, const Vector3& lineStart, const Vector3& lineEnd)
{
	const Vector3 diff = point - lineStart;

	const Vector3 lineDir    = lineEnd - lineStart;
	const Vector3 lineDirN   = lineDir.GetNormalize();
	const float   lineLength = lineDir.Magnitude();

	float projection = diff.Dot(lineDirN);

	projection = std::clamp(projection, 0.0f, lineLength);

	return lineStart + lineDirN * projection;
}

MATRIX Geometry::GetMatXYZ(const float x, const float y, const float z)
{
	auto xRot = MGetRotX(x);
	auto yRot = MGetRotX(y);
	auto zRot = MGetRotX(z);

	return MMult(MMult(xRot,yRot),zRot);
}

float Geometry::Easing(RangeLimitedValue<float, 0.0f, 1.0f> time, EasingKind kind)
{
	switch (kind)
	{
	case Geometry::EasingKind::kOutQuart:
		// https://easings.net/ja#easeOutQuart
		return 1 - static_cast<float>(pow(1 - time.Value(), 4));
		break;
	case Geometry::EasingKind::kInOutQuint:
		// https://easings.net/ja#easeInOutQuint
		return (time.Value() < 0.5f) ? 16 * static_cast<float>(pow(time.Value(), 5)) : 1 - static_cast<float>(pow(-2 * time.Value() + 2, 5)) / 2;
		break;
	case Geometry::EasingKind::kInExpo:
		// https://easings.net/ja#easeInExpo
		return time.IsMin() ? 0.0f : std::pow(2, 10 * time.Value() - 10);
		break;
	case Geometry::EasingKind::kOutElastic:
		// https://easings.net/ja#easeOutElastic
		if (time.IsMin()) return 0.0f;
		if (time.IsMax()) return 1.0f;
		return std::pow(2, -10 * time.Value()) * std::sinf((time.Value() * 10 - 0.75) * ((2 * kPi) / 3)) + 1;
		break;
	default:
		assert(false && "列挙に処理が作られていない");
		return time.Value();
		break;
	}
}
