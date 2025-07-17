#pragma once
#include "ActorKind.h"
#include "Vector3.h"
#include <list>
#include <memory>
#include "LockOnstate.h"

class Collidable;
class Rigid;
class Collider3D;
class Actor;

using SpawnActorList_t = std::list<std::shared_ptr<Actor>>;

// ゲーム内に登場する物体
// GameObjectだと「全部やん」ってなるので
class Actor abstract : public LockOnState
{
public:
	Actor(const bool canLockOn);
	virtual ~Actor();

	void ClearActorList();
	virtual void Update() abstract;
	virtual void Draw() const abstract;

	const Vector3&  GetPos()  const { return m_pos; }
	bool        CanCollide() const;
	Collidable& GetCollidable() const;
	bool        HasCol() const;
	Collider3D& GetCol() const;
	bool        HasRigid() const;
	Rigid&      GetRigid() const;
	const ActorKind GetKind() const;
	const bool      IsAlive() const;
	virtual void    OnCollisionEnter(std::shared_ptr<Actor> other) {}
	virtual void    OnCollisionStay(std::shared_ptr<Actor> other) {}
	virtual void    OnCollisionExit(std::shared_ptr<Actor> other) {}
	// 押し戻し後の移動量を反映させる時間
	virtual void    CommitMove() abstract;

	// 何かActorを生成したか
	const bool IsSpawnedActor() const;
	SpawnActorList_t GetSpawnActor();

	// ※このゲーム限定の機能
	// ライブラリとして使いまわすときは削除してね
	void LimitMovementRange();

protected:
	Vector3 m_pos;
	bool m_isAlive;
	std::shared_ptr<Collidable> m_collidable;
	ActorKind m_kind;
	// Controllerがこれを確認してオブジェクトリストに追加する
	// Actorから何か申し出る必要はない
	SpawnActorList_t m_spawnActorList;

protected:
	// Actorを生成する
	// 生成したものはActorControllerに渡され、次のフレームから動き出す
	void SpawnActor(std::shared_ptr<Actor> spawnActor);
};