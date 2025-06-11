#include "Actor.h"
#include "Collidable.h"

Actor::Actor() :
	m_isAlive(true),
	m_pos(),
	m_collidable(nullptr),
	m_kind(ActorKind::kNone)
{
}

Actor::~Actor()
{
}

void Actor::ClearActorList()
{
	m_spawnActorList.clear();
}

bool Actor::HasCollidable() const
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
	// ãÛÇ∂Ç·Ç»Ç©Ç¡ÇΩÇÁê∂ê¨ÇµÇƒÇÈ
	return !m_spawnActorList.empty();
}

SpawnActorList_t Actor::GetSpawnActor()
{
	// ï‘Ç∑ëOÇ…sort
	m_spawnActorList.sort();
	return m_spawnActorList;
}

void Actor::SpawnActor(std::shared_ptr<Actor> spawnActor)
{
	m_spawnActorList.emplace_back(spawnActor);
}
