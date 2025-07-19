#include "Actor.h"
#include "Collidable.h"

namespace
{
	constexpr float kFieldRadius = 2000.0f;
}

Actor::Actor(const bool canLockOn) :
	m_isAlive(true),
	m_pos(),
	m_collidable(nullptr),
	m_kind(ActorKind::kNone),
	LockOnState(canLockOn),
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
	// 空じゃなかったら生成してる
	return !m_spawnActorList.empty();
}

SpawnActorList_t Actor::GetSpawnActor()
{
	// 返す前にsort
	m_spawnActorList.sort();
	return m_spawnActorList;
}

void Actor::LimitMovementRange()
{
	//地面にめり込まないようにする
	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	// 円形のフィールドから出ないようにする
	const Vector3 nextPos = m_pos + m_collidable->GetVel();

	// 移動後一定のエリアから出ていたら
	if (nextPos.SqrMagnitude() > kFieldRadius * kFieldRadius)
	{
		// それを阻止するように移動速度を変えたい

		// 補正後の位置
		const Vector3 radiusDir = nextPos.GetNormalize() * kFieldRadius;

		// 速度にして設定
		m_collidable->SetVel(radiusDir - m_pos);
	}
}

bool Actor::IsStopUpdate() const
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
