#pragma once
#include <memory>
#include <list>
#include <unordered_map>

class Actor;
class Physics;

using ActorList_t = std::list<std::shared_ptr<Actor>>;

// Actorをまとめて処理する
// 当たり判定も捜査する
class ActorController
{
public:
	void Init();

	void AddActor(std::shared_ptr<Actor> addActor);
	void Update();
	void Draw() const;
	// 指定されたオブジェクトが自身の中に存在するか確かめる
	bool CheckIsAlive(std::shared_ptr<Actor> checkAct);
	ActorList_t SearchCanLockOnActor() const;
	// ロックオン可能な中で、指定された位置に最も近い位置を持つActorを返します
	std::shared_ptr<Actor> GetNearestLockOnActor(const Vector3& pos) const;

private:
	ActorList_t m_actors;
	// 重力、当たり判定、押し戻し
	std::shared_ptr<Physics> m_physics;
};