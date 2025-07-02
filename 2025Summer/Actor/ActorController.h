#pragma once
#include <memory>
#include <list>
#include <unordered_map>

class Actor;
class Physics;

using ActorList_t = std::list<std::shared_ptr<Actor>>;

// Actor���܂Ƃ߂ď�������
// �����蔻����{������
class ActorController
{
public:
	void Init();

	void AddActor(std::shared_ptr<Actor> addActor);
	void Update();
	void Draw() const;
	// �w�肳�ꂽ�I�u�W�F�N�g�����g�̒��ɑ��݂��邩�m���߂�
	bool CheckIsAlive(std::shared_ptr<Actor> checkAct);
	ActorList_t SearchCanLockOnActor() const;
	// ���b�N�I���\�Ȓ��ŁA�w�肳�ꂽ�ʒu�ɍł��߂��ʒu������Actor��Ԃ��܂�
	std::shared_ptr<Actor> GetNearestLockOnActor(const Vector3& pos) const;

private:
	ActorList_t m_actors;
	// �d�́A�����蔻��A�����߂�
	std::shared_ptr<Physics> m_physics;
};