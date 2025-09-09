#include "Actor.h"
#include "../Physics/Collidable.h"
#include "../Physics//Collider/Collider3D.h"

namespace
{
	constexpr float kFieldRadius = 2000.0f;
}

Actor::Actor(const bool canLockOn) :
	m_isAlive(true),
	m_pos(),
	m_collidable(nullptr),
	m_kind(ActorKind::kNone),
	ActorAttribute(canLockOn),
	m_stopFrame(0)
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

bool Actor::IsGround() const
{
	return m_collidable->GetCol().IsGround();
}

const ActorKind Actor::GetKind() const
{
	return m_kind;
}

const bool Actor::IsAlive() const
{
	return m_isAlive;
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

void Actor::SetStopFrame(const int frame)
{
	m_stopFrame = std::max(m_stopFrame, frame);
}

bool Actor::IsStop() const
{
	return m_stopFrame;
}

void Actor::CountStopFrame()
{
	--m_stopFrame;
	if (m_stopFrame < 0) m_stopFrame = 0;
}

void Actor::Destroy()
{
	m_isAlive = false;
}

void Actor::SpawnActor(std::shared_ptr<Actor> spawnActor)
{
	m_spawnActorList.emplace_back(spawnActor);
}
