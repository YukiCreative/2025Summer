#pragma once
#include <memory>
#include <list>

class Actor;
class Collider3D;

// コライダーを動かし、当たり判定を行い、押し戻しを計算させる
// ActorControllerが持つ
class Physics
{
public:

	// ActorControllerから移動と当たり判定を分離した
	void Update(std::list<std::shared_ptr<Actor>> actorList);

	// 当たり判定の範囲を描画
	// デバッグ用
	void DrawColRange(std::list <std::shared_ptr<Actor>>) const;
private:
	// 重力を処理
	void Gravity(std::list<std::shared_ptr<Actor>> actorList);
	// 当たり判定
	void CheckHit(std::list<std::shared_ptr<Actor>>& actorList);
};

