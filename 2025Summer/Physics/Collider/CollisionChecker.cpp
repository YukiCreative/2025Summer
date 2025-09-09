#include "../../Actor/Actor.h"
#include "CapsuleCollider.h"
#include "../Collidable.h"
#include "CollisionChecker.h"
#include "../../Geometry/Geometry.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "../Rigid.h"
#include <algorithm>
#include <array>
#include <DxLib.h>

float CollisionChecker::WeightRate(Collidable& colA, Collidable& colB)
{
	float weightRate = static_cast<float>(colB.GetWeight()) / (static_cast<float>(colA.GetWeight()) + static_cast<float>(colB.GetWeight())); // 球体が受ける移動量の割合

	// もしAかBどちらかがstaticなら、そいつは動かずに
	// 動く方にすべてを押し付ける
	if (colA.IsStatic())
	{
		weightRate = 0.0f;
	}
	else if (colB.IsStatic())
	{
		weightRate = 1.0f;
	}
	// どっちもstaticならもう動くな
	if (colA.IsStatic() && colB.IsStatic())
	{
		colA.SetVel(Vector3::Zero());
		colB.SetVel(Vector3::Zero());
	}

	return weightRate;
}

bool CollisionChecker::CheckHitSS(const Collidable& colA, const Collidable& colB)
{
	auto& sphereColA = static_cast<SphereCollider&>(colA.GetCol());
	auto& sphereColB = static_cast<SphereCollider&>(colB.GetCol());

	// 移動後の位置を調べる
	const auto nextA = colA.GetPos() + colA.GetVel();
	const auto nextB = colB.GetPos() + colB.GetVel();

	const auto radiusSum = sphereColA.GetRadius() + sphereColB.GetRadius();

	return (nextA - nextB).SqrMagnitude() < radiusSum * radiusSum;
}

void CollisionChecker::FixMoveSS(Collidable& colA, Collidable& colB)
{
	// 移動後の位置
	const auto nextA = colA.GetPos() + colA.GetVel();
	const auto nextB = colB.GetPos() + colB.GetVel();

	auto& sphereColA = static_cast<SphereCollider&>(colA.GetCol());
	auto& sphereColB = static_cast<SphereCollider&>(colB.GetCol());

	const auto radiusSum = sphereColA.GetRadius() + sphereColB.GetRadius();

	const auto atoBN = (nextB - nextA).GetNormalize();

	// めり込んだ分離れる
	const float overlapLength = radiusSum - (nextA - nextB).Magnitude();

	const Vector3 overlap = atoBN * overlapLength;

	const auto weightRate = WeightRate(colA, colB);

	// 算出した値を反映
	colA.AddVel(-overlap * weightRate);
	colB.AddVel(overlap * (1 - weightRate));
}

MV1_COLL_RESULT_POLY_DIM CollisionChecker::CheckHitMS(Collidable& mCol, Collidable& sCol)
{
	MeshCollider& mesh = static_cast<MeshCollider&>(mCol.GetCol());
	SphereCollider& sphere = static_cast<SphereCollider&>(sCol.GetCol());

	const Vector3 sphereNextPos = sphere.GetPos() + sCol.GetRigid().GetVel();

	return MV1CollCheck_Sphere(mesh.GetModelHadle(), mesh.GetCollisionFrameIndex(), sphereNextPos, sphere.GetRadius());
}

void CollisionChecker::FixMoveMS(Collidable& mCol, Collidable& sCol, DxLib::tagMV1_COLL_RESULT_POLY_DIM hitData)
{
	SphereCollider& sphere = static_cast<SphereCollider&>(sCol.GetCol());
	const Vector3 sphereNextPos = sphere.GetPos() + sCol.GetRigid().GetVel();

	// 一番最初の当たったポリゴンのみに対して押し戻す
	auto aPolygon = hitData.Dim[0];

	const Vector3 normal = aPolygon.Normal;
	// 三つのうち一つの適当な頂点
	const Vector3 aVertexPos = aPolygon.Position[0];
	const Vector3 vertexToSphere = sphereNextPos - aVertexPos;

	const float hitLength = abs(normal.Dot(vertexToSphere)) / normal.Magnitude();

	const Vector3 overlap = normal * (sphere.GetRadius() - hitLength);

	const float weightRate = WeightRate(mCol, sCol);

	mCol.AddVel(-overlap * weightRate);
	sCol.AddVel(overlap * (1.0f - weightRate));
}

DxLib::tagMV1_COLL_RESULT_POLY_DIM CollisionChecker::CheckHitMC(Collidable& mCol, Collidable& cCol)
{
	MeshCollider& mesh = static_cast<MeshCollider&>(mCol.GetCol());
	CapsuleCollider& capsule = static_cast<CapsuleCollider&>(cCol.GetCol());

	const Vector3 nextStartPos = capsule.StartPos() + cCol.GetRigid().GetVel();
	const Vector3 nextEndPos = capsule.EndPos() + cCol.GetRigid().GetVel();

	return MV1CollCheck_Capsule(mesh.GetModelHadle(), mesh.GetCollisionFrameIndex(), nextStartPos, nextEndPos, capsule.GetRadius());
}

void CollisionChecker::FixMoveMC(Collidable& mCol, Collidable& cCol, DxLib::tagMV1_COLL_RESULT_POLY_DIM hitData)
{
	CapsuleCollider& capsule = static_cast<CapsuleCollider&>(cCol.GetCol());

	const Vector3 nextStartPos = capsule.StartPos() + cCol.GetRigid().GetVel();
	const Vector3 nextEndPos = capsule.EndPos() + cCol.GetRigid().GetVel();

	// 1.平行かそうでないかを調べる
	// 2.平行でないなら、startからendに球体が移動して、ちょうど接している二点を探す
	// 3.法線との内積をとって、表側の点を採用
	// 4.あとはそれがstartなのかendなのかを、なんかいい感じに出して適用
	// これを当たらなくなるまでor上限回数やる

	for (int i = 0; i < hitData.HitNum; ++i)
	{
		MV1_COLL_RESULT_POLY aPolygon = hitData.Dim[i];

		if (!aPolygon.HitFlag) continue;

		Vector3 normal = aPolygon.Normal;
		Vector3 overlap{};
		// 平行なら
		if (abs(capsule.Direction().Dot(normal)) < Geometry::kEpsilon)
		{
			// どの点をとっても同じなので端っこを適当に採用
			// あとは球体と面の押し戻し
			const Vector3 aVertexPos = aPolygon.Position[0];
			const Vector3 vertexToSphere = nextStartPos - aVertexPos;

			const float hitLength = abs(normal.Dot(vertexToSphere)) / normal.Magnitude();

			overlap = normal * (capsule.GetRadius() - hitLength);
		}
		else
		{
			// そうでないなら
			// 線分と面の当たり判定
			
			// 十分に余分に長くした座標を作る
			const Vector3 addRadiusStartPos = nextStartPos - capsule.Direction() * (capsule.GetRadius() * 10.0f);
			const Vector3 addRadiusEndPos = nextEndPos + capsule.Direction() * (capsule.GetRadius() * 10.0f);

			// その線分と当たっている点を出す
			HITRESULT_LINE hitResult = HitCheck_Line_Triangle(addRadiusStartPos, addRadiusEndPos, aPolygon.Position[0], aPolygon.Position[1], aPolygon.Position[2]);

			// カプセルは当たっているが、線分は当たっていなかった
			// こんな時は他のポリゴンが押し戻してくれるはず
			if (!hitResult.HitFlag) continue;

			// それに近いほうの端点から当たっている点までのベクトルを、法線方向に射影
			float dot = 0;
			if ((nextStartPos - hitResult.Position).SqrMagnitude() < (nextEndPos - hitResult.Position).SqrMagnitude())
			{
				 dot = (Vector3(hitResult.Position) - nextStartPos).Dot(normal);
			}
			else
			{
				dot = (Vector3(hitResult.Position) - nextEndPos).Dot(normal);
			}

			overlap = normal * (dot + capsule.GetRadius());
		}

		const float weightRate = WeightRate(mCol, cCol);

		mCol.AddVel(-overlap * weightRate);
		cCol.AddVel(overlap * (1.0f - weightRate));
	}
}

bool CollisionChecker::CheckHitCS(const Collidable& cCol, const Collidable& sCol)
{
	auto& sphereCol = static_cast<SphereCollider&>(sCol.GetCol());
	auto& capsuleCol = static_cast<CapsuleCollider&>(cCol.GetCol());

	// 球の中心とカプセル線分との最近接点を出す

	const Vector3 sphereNextPos = sphereCol.GetPos() + sCol.GetRigid().GetVel();
	const Vector3 capsuleNextStartPos = capsuleCol.StartPos() + cCol.GetRigid().GetVel();

	const Vector3 startToSphere = sphereNextPos - capsuleNextStartPos;
	const Vector3 capsuleDir = capsuleCol.Direction(); // 向きは移動量を含んでも変わらんだろ

	float projection = startToSphere.Dot(capsuleDir);

	// projectionを線分の長さまでに制限
	projection = std::clamp(projection, 0.0f, capsuleCol.Length());

	const Vector3 nearestPosOnLine = capsuleNextStartPos + capsuleDir * projection;

	const float radiusSum = sphereCol.GetRadius() + capsuleCol.GetRadius();

	// 球の中心→最近接点がそれぞれの半径の合計より短ければ当たってる
	return (nearestPosOnLine - sphereCol.GetPos()).SqrMagnitude() < radiusSum * radiusSum;
}

void CollisionChecker::FixMoveCS(Collidable& cCol, Collidable& sCol)
{
	// 当たらない位置まで速度を補正


	auto& sphereCol = static_cast<SphereCollider&>(sCol.GetCol());
	auto& capsuleCol = static_cast<CapsuleCollider&>(cCol.GetCol());

	// 球の中心とカプセル線分との最近接点をもう一回出す

	const Vector3 sphereNextPos = sphereCol.GetPos() + sCol.GetRigid().GetVel();
	const Vector3 capsuleNextStartPos = capsuleCol.StartPos() + cCol.GetRigid().GetVel();

	const Vector3 startToSphere = sphereNextPos - capsuleNextStartPos;
	const Vector3 capsuleDir = capsuleCol.Direction(); // 向きは移動量を含んでも変わらんだろ

	float projection = startToSphere.Dot(capsuleDir);

	// projectionを線分の長さまでに制限
	projection = std::clamp(projection, 0.0f, capsuleCol.Length());

	const Vector3 nearestPosOnLine = capsuleNextStartPos + capsuleDir * projection;

	// めり込んでいるベクトルがほしい

	auto sphereToNearest = nearestPosOnLine - sphereNextPos;
	auto sphereToNearestN = sphereToNearest.GetNormalize();

	auto sphereToNearestLength = sphereToNearest.Magnitude();

	auto radiusSum = capsuleCol.GetRadius() + sphereCol.GetRadius();

	auto diff = radiusSum - sphereToNearestLength;

	const Vector3 overlap = sphereToNearestN * diff;

	DrawLine3D({0,0,0}, overlap, 0xff0000);
	DrawSphere3D(nearestPosOnLine, 10, 10, 0xffffff, 0xffffff, true);

	// めり込んだベクトルを、それぞれの重さの比率で分配

	float weightRate = static_cast<float>(cCol.GetWeight()) / (static_cast<float>(cCol.GetWeight()) + static_cast<float>(sCol.GetWeight())); // 球体が受ける移動量の割合

	// もしAかBどちらかがstaticなら、そいつは動かずに
	// 動く方にすべてを押し付ける
	if (cCol.IsStatic())
	{
		weightRate = 1.0f;
	}
	else if (sCol.IsStatic())
	{
		weightRate = 0.0f;
	}
	// どっちもstaticならもう動くな
	if (cCol.IsStatic() && sCol.IsStatic())
	{
		cCol.SetVel(Vector3::Zero());
		sCol.SetVel(Vector3::Zero());
		return;
	}

	cCol.AddVel(overlap * (1.0f - weightRate));
	sCol.AddVel(-overlap * weightRate);

}

bool CollisionChecker::CheckHitCC(const Collidable& colA, const Collidable& colB)
{
	auto& cColA = static_cast<CapsuleCollider&>(colA.GetCol());
	auto& cColB = static_cast<CapsuleCollider&>(colB.GetCol());

	const auto velA = colA.GetVel();
	const auto velB = colB.GetVel();

	// DxLibを使わせていただく
	return HitCheck_Capsule_Capsule(cColA.StartPos() + velA, cColA.EndPos() + velA, cColA.GetRadius(), cColB.StartPos() + velB, cColB.EndPos() + velB, cColB.GetRadius());
}

void CollisionChecker::FixMoveCC(Collidable& colA, Collidable& colB)
{
	// 最近接点をそれぞれ中心に持った球の押し戻し

	auto& cColA = static_cast<CapsuleCollider&>(colA.GetCol());
	auto& cColB = static_cast<CapsuleCollider&>(colB.GetCol());

	float t, s;
	Vector3 minPos1, minPos2;

	const Vector3 velA       = colA.GetVel();
	const Vector3 velB       = colB.GetVel();
	const Vector3 nextPosA   = cColA.GetPos()   + velA;
	const Vector3 nextStartA = cColA.StartPos() + velA;
	const Vector3 nextEndA   = cColA.EndPos()   + velA;
	const Vector3 nextPosB   = cColB.GetPos()   + velB;
	const Vector3 nextStartB = cColB.StartPos() + velB;
	const Vector3 nextEndB   = cColB.EndPos()   + velB;

	// 正規化されていない線分が必要
	const Vector3 dirA = nextEndA - nextPosA;
	const Vector3 dirB = nextEndB - nextPosB;

	const Vector3 deltaPos = nextPosB - nextPosA;
	const Vector3 dirNormal = dirA.Cross(dirB);

	// カプセルが平行かどうか
	if (dirNormal.SqrMagnitude() < 0.01f)
	{
		// 平行

		// Aの適当な端点から、一番近いBの点を求める
		// そこから、一番近いAの点を求める
		// 出た二つの点が最近接点

		minPos2 = Geometry::PointSegmentNearestPos(nextStartA, nextStartB, nextEndB);

		minPos1 = Geometry::PointSegmentNearestPos(minPos2, nextStartA, nextEndA);
	}
	else
	{
		// 平行でない

		// 行列で連立方程式が解けるらしい
		MATRIX matSolve = MGetAxis1
		(
			 dirA,
			-dirB,
			dirNormal,
			{0,0,0}
		);

		matSolve = MInverse(matSolve);

		s = Vector3{ matSolve.m[0][0],matSolve.m[1][0] ,matSolve.m[2][0] }.Dot(deltaPos);
		t = Vector3{ matSolve.m[0][1],matSolve.m[1][1] ,matSolve.m[2][1] }.Dot(deltaPos);

		// clamp
		s = std::clamp(s, -1.0f, 1.0f);
		t = std::clamp(t, -1.0f, 1.0f);

		minPos1 = nextPosA + dirA * s;
		minPos2 = nextPosB + dirB * t;

		// 最近接点が端になった時の挙動がおかしかったので修正
		if (s <= -0.9999f || s >= 0.9999f)
		{
			// 計算しなおす
			minPos2 = Geometry::PointSegmentNearestPos(minPos1, nextStartB, nextEndB);
		}
		if (t <= -0.9999f || t >= 0.9999f)
		{
			minPos1 = Geometry::PointSegmentNearestPos(minPos2, nextStartA, nextEndA);
		}
	}

	DrawSphere3D(minPos1, 10,10, 0xffffff,0xffffff, true);
	DrawSphere3D(minPos2, 10,10, 0xffffff,0xffffff, true);

	// 後は球体の押し戻しと同じ
	// めり込んでいるベクトルがほしい

	const Vector3 nearestPosDiff = minPos2 - minPos1;
	const Vector3 nearestPosDiffN = nearestPosDiff.GetNormalize();

	const float radiusSum = cColA.GetRadius() + cColB.GetRadius();
	const float rerativeLength = nearestPosDiff.Magnitude();
	const float diff = radiusSum - rerativeLength;

	const Vector3 overlap = nearestPosDiffN * diff;

	const float weightRate = WeightRate(colA, colB);

	colA.AddVel(-overlap * weightRate);
	colB.AddVel(overlap * (1.0f - weightRate));
}

bool CollisionChecker::CheckIsGround(DxLib::tagMV1_COLL_RESULT_POLY_DIM hitData)
{
	// この内積値まで床判定
	constexpr float kFloorThreshold = 0.7f;

	bool result = false;

	for (int i = 0; i < hitData.HitNum; ++i)
	{
		MV1_COLL_RESULT_POLY aPolygon = hitData.Dim[i];
		result |= Vector3(aPolygon.Normal).Dot(Vector3::Up()) > kFloorThreshold;
	}
	return result;
}
