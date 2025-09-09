#pragma once
#include <array>

class Actor;
class Collidable;
class Physics;

namespace DxLib
{
	struct tagMV1_COLL_RESULT_POLY_DIM;
	struct tagMV1_REF_POLYGONLIST;
}

// 衝突判定の関数をまとめました
// Physicsに書くと長いので
class CollisionChecker
{
	friend Physics;
private:
	
	// 便利関数

	// 第一引数の重さの比重を計算
	static float WeightRate(Collidable& colA, Collidable& colB);

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
	static bool CheckHitSS(const Collidable& colA, const Collidable& colB);
	/// <param name="colA">球体A</param>
	/// <param name="colB">球体B</param>
	/// <param name="time">衝突時間</param>
	static void FixMoveSS(Collidable& colA, Collidable& colB);

	// ポリゴンx球
	static DxLib::tagMV1_COLL_RESULT_POLY_DIM CheckHitMS(Collidable& mCol, Collidable& sCol);
	// hitDataの解放も行う
	static void FixMoveMS(Collidable& mCol, Collidable& sCol, DxLib::tagMV1_COLL_RESULT_POLY_DIM hitData);

	// ポリゴンxカプセル
	static DxLib::tagMV1_COLL_RESULT_POLY_DIM CheckHitMC(Collidable& mCol, Collidable& cCol);
	static void FixMoveMC(Collidable& mCol, Collidable& cCol, DxLib::tagMV1_COLL_RESULT_POLY_DIM hitData);

	// カプセルx球
	// 貫通は検知できません
	static bool CheckHitCS(const Collidable& cCol, const Collidable& sCol);
	static void FixMoveCS(Collidable& cCol, Collidable& sCol);

	// カプセルxカプセル
	static bool CheckHitCC(const Collidable& colA, const Collidable& colB);
	static void FixMoveCC(Collidable& colA,  Collidable& sCol);

	// このポリゴンに床ポリゴンが含まれているかを返してるだけ
	static bool CheckIsGround(DxLib::tagMV1_COLL_RESULT_POLY_DIM hitData);

	// 第一引数の点から垂直に見て一番近い面の法線を返します
	// 下に面がないときは0を返します
	static Vector3 GetGroundNormal(const Vector3& startPos, const Collidable& mCol);
};