#include "Actor.h"
#include "Collidable.h"

Actor::Actor(const bool canLockOn) :
	m_isAlive(true),
	m_pos(),
	m_collidable(nullptr),
	m_kind(ActorKind::kNone),
	LockOnState(canLockOn)
{
}

Actor::~Actor()
{
}

void Actor::ClearActorList()
{
	m_spawnActorList.clear();
}

bool Actor::CanCollide() const
{
	return static_cast<bool>(m_collidable);
}

Collidable& Actor::GetCollidable() const
{
	return *m_collidable;
}

bool Actor::HasCol() const
{
	return m_collidable->HasCol();
}

Collider3D& Actor::GetCol() const
{
	return m_collidable->GetCol();
}

bool Actor::HasRigid() const
{
	return m_collidable->HasRigid();
}

Rigid& Actor::GetRigid() const
{
	return m_collidable->GetRigid();
}

const ActorKind Actor::GetKind() const
{
	return m_kind;
}

const bool Actor::IsAlive() const
{
	return m_isAlive;
}

void Actor::OnCollision(std::shared_ptr<Actor> other)
{
}

const bool Actor::IsSpawnedActor() const
{
	// 空じゃなかったら生成してる
	return !m_spawnActorList.empty();
}

SpawnActorList_t Actor::GetSpawnActor()
{
	// 返す前にsort
	m_spawnActorList.sort();
	return m_spawnActorList;
}

void Actor::SpawnActor(std::shared_ptr<Actor> spawnActor)
{
	m_spawnActorList.emplace_back(spawnActor);
}
