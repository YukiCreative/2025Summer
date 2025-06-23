#include "CollisionChecker.h"
#include <DxLib.h>
#include "Actor.h"
#include "Collidable.h"
#include "Rigid.h"
//#include "Collider3D.h"
#include <array>
#include "CapsuleCollider.h"
#include <algorithm>

bool CollisionChecker::CheckHitSS(const Collidable& colA, const Collidable& colB, float& time)
{
	auto& sphereColA = static_cast<SphereCollider&>(colA.GetCol());
	auto& sphereColB = static_cast<SphereCollider&>(colB.GetCol());

	const Vector3 initColDist  = colB.GetPos() - colA.GetPos(); // 移動前相対距離
	const Vector3 afterColA    = colA.GetPos() + colA.GetVel(); // 移動後A
	const Vector3 afterColB    = colB.GetPos() + colB.GetVel(); // 移動後B
	const Vector3 afterColDist = afterColB     - afterColA;     // 移動後相対距離
	const Vector3 colDistDiff  = afterColDist  - initColDist;   // 相対距離の差分
	const float sumRad = sphereColA.GetRadius() + sphereColB.GetRadius();

	// 最初から衝突しているかどうか
	if (initColDist.SqrMagnitude() <= sumRad * sumRad)
	{
		time = 0;
		return true;
	}

	// 平行移動しているか
	if (colDistDiff.SqrMagnitude() == 0)
	{
		// 実はすでに上で平行移動かつ衝突の場合をキャッチできているので、ここに来るということは当たっていない
		return false;
	}

	const float dot = initColDist.Dot(colDistDiff);	// いい名前がつけられない
	const float colDistLength = initColDist.SqrMagnitude(); // 移動前相対距離の長さ(平方根で割っていない)
	const float colDistDiffLength = colDistDiff.SqrMagnitude();

	// 衝突の判別式
	const float discriminant = dot*dot-colDistDiffLength*(colDistLength-sumRad*sumRad);
	// √の中が負の数なら解なし
	if (discriminant < 0)
	{
		return false;
	}

	time = (-dot - sqrtf(discriminant)) / colDistDiffLength;
	// このフレームでは当たらないパターン
	if (time < 0 || time > 1)
	{
		return false;
	}

	// ここまで来たら当たっている　おめでとう
	return true;
}

void CollisionChecker::FixMoveSS(Collidable& colA, Collidable& colB, const float time)
{
	const Vector3 velA = colA.GetVel();
	const Vector3 velB = colB.GetVel();

	// 二つの球体が、最初に接触する位置を出す
	const Vector3 fixedColAPos = colA.GetPos() + velA * time;
	const Vector3 fixedColBPos = colB.GetPos() + velB * time;
	// AからBのベクトル
	// これが球体の衝突の法線
	const Vector3 diffN = (fixedColBPos - fixedColAPos).GetNormalize();
	// AとBのパラメータから、反発率を決める
	const float reboundRate = 1 + colA.GetBounce() * colB.GetBounce();
	// 重量たしたやつ
	const float totalWeight = static_cast<float>(colA.GetWeight() + colB.GetWeight());
	// 式を分割してるだけ
	const float dot = (velA - velB).Dot(diffN);
	float weightFactA = -colB.GetWeight() / totalWeight;
	float weightFactB =  colA.GetWeight() / totalWeight;
	const Vector3 constVec = diffN * (reboundRate * dot);

	// もしAかBどちらかがstaticなら、そいつは動かずに
	// 動く方にすべてを押し付ける
	if (colA.IsStatic())
	{
		weightFactA = 0;
		weightFactB = 1;
	}
	else if (colB.IsStatic())
	{
		weightFactA = 1;
		weightFactB = 0;
	}
	// どっちもstaticならもう動くな
	if (colA.IsStatic() && colB.IsStatic())
	{
		weightFactA = 0;
		weightFactB = 0;
	}

	// 衝突後ベクトルの算出
	const Vector3 fixedVelA = constVec * weightFactA + velA;
	const Vector3 fixedVelB = constVec * weightFactB + velB;

	// 算出した値を反映
	colA.SetVel(fixedVelA);
	colB.SetVel(fixedVelB);
	colA.SetPos(fixedColAPos);
	colB.SetPos(fixedColBPos);
}

void CollisionChecker::ComparePolyHit(PolyHitData& a, const PolyHitData& b)
{
	if (a.time < b.time) return;

	a = b;
}

bool CollisionChecker::CheckHitSP(const Collidable& sCol, const Collidable& pCol, PolyHitData& hit)
{
	auto& sphereCol = static_cast<SphereCollider&>(sCol.GetCol());
	auto& polygonCol = static_cast<PolygonCollider&>(pCol.GetCol());

	bool isHit = false;

	const Vector3 sumVel = sCol.GetVel() - pCol.GetVel(); // 球体から見た相対速度で考える

	// まず、ポリゴンのメッシュの状態を取得する
	// ための更新処理
	polygonCol.RefreshRefMesh();
	auto& refMesh = polygonCol.GetRefMesh();

	// 取得したポリゴンすべてに対して実行(やばそう)
	for (int i = 0; i < refMesh.PolygonNum; ++i)
	{
		// とりあえず必要な情報を取得
		const auto& polygon = refMesh.Polygons[i];
		const auto& vertices = refMesh.Vertexs;
		// 頂点の座標
		const std::array<Vector3, 3> polygonVertices =
		{
			vertices[polygon.VIndex[0]].Position,
			vertices[polygon.VIndex[1]].Position,
			vertices[polygon.VIndex[2]].Position,
		};
		DrawSphere3D(polygonVertices[0], 100, 10, 0xff0000, 0xff0000, true);
		DrawSphere3D(polygonVertices[1], 100, 10, 0xff0000, 0xff0000, true);
		DrawSphere3D(polygonVertices[2], 100, 10, 0xff0000, 0xff0000, true);
		// 辺
		const std::array<Vector3, 3> polygonLines =
		{
			polygonVertices[1] - polygonVertices[0], // 0~1に向かうベクトル
			polygonVertices[2] - polygonVertices[1], // 1~2
			polygonVertices[0] - polygonVertices[2], // 2~0
		};
		// (面の)法線
		const Vector3 normal = polygonLines[2].Cross(polygonLines[0]).GetNormalize();
		DrawLine3D(pCol.GetPos(), pCol.GetPos() + normal * 500, 0xffffff);
		const float dot = sumVel.Dot(normal); // いまの移動方向と面の向きの関係がどれぐらいかがわかる
		const Vector3 c0 = sphereCol.GetPos() - polygonVertices[0]; // どこか適当なポリゴンの頂点から球の現在位置まで
		const float dotC0 = c0.Dot(normal);
		const float distPlaneToPoint = fabsf(dotC0); // 面から点への距離

		const float collideTime = (sphereCol.GetRadius() - dotC0) / dot; // これが衝突時間

		if (dot >= 0) continue; // 移動方向が面に対して離れる
		if (collideTime > 1 || collideTime < 0) continue; // 今回の移動で衝突しない場合

		// 衝突した
		// より近い衝突だった場合のみ更新
		ComparePolyHit(hit, PolyHitData(collideTime, polygonVertices, normal));
		//isHit = true;
	}

	return isHit;
}

void CollisionChecker::FixMoveSP(Collidable& _sphereCol, Collidable& _polygonCol, const PolyHitData& hitData)
{
	auto& sphereCol = static_cast<SphereCollider&>(_sphereCol.GetCol());
	auto& polygonCol = static_cast<PolygonCollider&>(_polygonCol.GetCol());

	// 場所を固定してみる
	_sphereCol.SetPos(_sphereCol.GetPos() + _sphereCol.GetVel() * hitData.time);
	_polygonCol.SetPos(_polygonCol.GetPos() + _polygonCol.GetVel() * hitData.time);

	// バウンド
	_sphereCol.GetRigid().SetVel(_sphereCol.GetVel() * (hitData.time - 1));
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

	cCol.SetVel(overlap * (1.0f - weightRate));
	sCol.SetVel(-overlap * weightRate);

}

// ===============================================

PolyHitData::PolyHitData() :
	time(kBigNum),
	vertices(),
	normal()
{
}

PolyHitData::PolyHitData(const float _time, const Vertices& _vertices, const Vector3& _normal)
{
	time = _time;
	vertices = _vertices;
	normal = _normal;
}

void PolyHitData::operator=(const PolyHitData& other)
{
	time = other.time;
	vertices = other.vertices;
	normal = other.normal;
}