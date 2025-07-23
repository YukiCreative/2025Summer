#pragma once
#include <memory>
#include <list>
#include <unordered_map>

class Actor;
class Physics;
class UIBase;

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
	ActorList_t SearchCanLockOnActor() const;
	// ���b�N�I���\�Ȓ��ŁA�w�肳�ꂽ�ʒu�ɍł��߂��ʒu������Actor��Ԃ��܂�
	// �����������E�ɓG�����Ȃ�������nullptr���Ԃ��Ă��܂�
	std::shared_ptr<Actor> SearchNearestLockOnActor(const Vector3& pos) const;
	// Enemy��list��Ԃ��܂�
	ActorList_t SearchEnemy() const;

private:
	ActorList_t m_actors;
	// �d�́A�����蔻��A�����߂�
	std::shared_ptr<Physics> m_physics;
};