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

// �Q�[�����ɓo�ꂷ�镨��
// GameObject���Ɓu�S�����v���ĂȂ�̂�
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
	// �����߂���̈ړ��ʂ𔽉f�����鎞��
	virtual void    CommitMove() abstract;

	// ����Actor�𐶐�������
	const bool IsSpawnedActor() const;
	SpawnActorList_t GetSpawnActor();

	// �����̃Q�[������̋@�\
	// ���C�u�����Ƃ��Ďg���܂킷�Ƃ��͍폜���Ă�
	void LimitMovementRange();

protected:
	Vector3 m_pos;
	bool m_isAlive;
	std::shared_ptr<Collidable> m_collidable;
	ActorKind m_kind;
	// Controller��������m�F���ăI�u�W�F�N�g���X�g�ɒǉ�����
	// Actor���牽���\���o��K�v�͂Ȃ�
	SpawnActorList_t m_spawnActorList;

protected:
	// Actor�𐶐�����
	// �����������̂�ActorController�ɓn����A���̃t���[�����瓮���o��
	void SpawnActor(std::shared_ptr<Actor> spawnActor);
};