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
	// �󂶂�Ȃ������琶�����Ă�
	return !m_spawnActorList.empty();
}

SpawnActorList_t Actor::GetSpawnActor()
{
	// �Ԃ��O��sort
	m_spawnActorList.sort();
	return m_spawnActorList;
}

void Actor::LimitMovementRange()
{
	//�n�ʂɂ߂荞�܂Ȃ��悤�ɂ���
	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	// �~�`�̃t�B�[���h����o�Ȃ��悤�ɂ���
	const Vector3 nextPos = m_pos + m_collidable->GetVel();

	// �ړ�����̃G���A����o�Ă�����
	if (nextPos.SqrMagnitude() > kFieldRadius * kFieldRadius)
	{
		// �����j�~����悤�Ɉړ����x��ς�����

		// �␳��̈ʒu
		const Vector3 radiusDir = nextPos.GetNormalize() * kFieldRadius;

		// ���x�ɂ��Đݒ�
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
