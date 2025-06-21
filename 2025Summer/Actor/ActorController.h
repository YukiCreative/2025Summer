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
	// これはこのゲーム独自のものだけど、ここ(使いまわしたいクラス)に書いてしまう
	// だって継承して新しいクラス作るのめんどくさいし
	ActorList_t SearchCanLockOnActor() const;

private:
	ActorList_t m_actors;
	// 重力、当たり判定、押し戻し
	std::shared_ptr<Physics> m_physics;
};