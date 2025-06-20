#pragma once

#include "SphereCollider.h"
#include "PolygonCollider.h"
#include <array>

class Actor;
class Collidable;
class Physics;

struct PolyHitData
{
	using Vertices = std::array<Vector3, 3>;
	const float kBigNum =3.4e38f;
	float time;
	// ポリゴンを形成する頂点3つ
	// 時計回りに頂点が入る
	Vertices vertices;
	Vector3 normal; // 法線。頂点から求まるけど、判定の途中で計算するので入れとく

	PolyHitData();
	PolyHitData(const float _time, const Vertices& _vertices, const Vector3& _normal);
	void operator=(const PolyHitData& other);
};

// 衝突判定の関数をまとめました
// Physicsに書くと長いので
class CollisionChecker
{
	friend Physics;
private:
	
	// CheckHit〇〇系
	// Colliderの種類に応じた当たり判定の処理
	// 当たったかどうかを返す　そのほかに返したいデータがあったら参照を要求する

	// FixMove〇〇系
	// 実際に当たる瞬間(当たらなくなる位置)まで移動させ、
	// さらにその後の移動速度をいじる

	// 球ｘ球
	/// <param name="colA">球体Ａ</param>
	/// <param name="colB">球体Ｂ</param>
	/// <param name="time">衝突時間を格納するポインタ(返す値の範囲0~1)</param>
	/// <returns>当たったかどうか</returns>
	static bool CheckHitSS(const Collidable& colA, const Collidable& colB, float& time);
	/// <param name="colA">球体A</param>
	/// <param name="colB">球体B</param>
	/// <param name="time">衝突時間</param>
	static void FixMoveSS(Collidable& colA, Collidable& colB, const float time);

	// 球体xポリゴン

	// AとBを比べて、より衝突時間が早い方をAに代入します。
	static void ComparePolyHit(PolyHitData& a, const PolyHitData& b);

	/// <param name="sCol">球体</param>
	/// <param name="pCol">ポリゴン</param>
	/// <param name="hit">一番近い衝突時間＆ポリゴンが返ってくる</param>
	/// <returns>当たったかどうか</returns>
	static bool CheckHitSP(const Collidable& sCol, const Collidable& pCol, PolyHitData& hit);
	static void FixMoveSP(Collidable& sphereCol, Collidable& polygonCol, const PolyHitData& hitData);

	// カプセルx球
	// 貫通は検知できません
	static bool CheckHitCS(const Collidable& cCol, const Collidable& sCol);
	static void FixMoveCS(Collidable& cCol, Collidable& sCol);
};