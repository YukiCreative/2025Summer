#include "Actor.h"
#include "ActorController.h"
#include "Physics.h"
#include <DxLib.h>
#include <list>
#include "Enemy.h"

void ActorController::Init()
{
	m_physics = std::make_shared<Physics>();
}

void ActorController::AddActor(std::shared_ptr<Actor> addActor)
{
	m_actors.emplace_back(addActor);
}

void ActorController::Update()
{
	m_beforeDeathEnemy.clear();

	ActorList_t aliveActors;
	for (auto& actor : m_actors)
	{
		actor->CountStopFrame();
		// ������~���Ȃ�Update���Ȃ�
		if (!actor->IsStop())
		{
			actor->Update();
		}
		actor->LimitMovementRange();

		// �����Ă����X�g�����
		if (actor->IsAlive())
		{
			aliveActors.emplace_back(actor);
		}
		else if (actor->GetKind() == ActorKind::kEnemy)
		{
			// �G�����񂾂Ƃ��A���񂾓G���L�^
			auto enemy = std::static_pointer_cast<Enemy>(actor);

			m_beforeDeathEnemy.emplace_back(enemy->GetEnemyKind());
		}

		// ����Actor�𐶐����Ă�����
		if (actor->IsSpawnedActor())
		{
			// ������E���Ă������ActorList�ŊǗ�����
			aliveActors.sort();
			aliveActors.merge(actor->GetSpawnActor());
			actor->ClearActorList();
		}
	}
	// m_actors�𐶂��Ă������ɂ���
	m_actors = aliveActors;

	// �d�͂Ƃ������蔻��Ƃ�
	m_physics->Update(m_actors);

	// �C����̈ړ��ʔ��f
	for (auto& actor : m_actors)
	{
		if (actor->IsStop()) continue;
		actor->CommitMove();
	}
}

void ActorController::Draw() const
{
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}

#if _DEBUG
	// �����蔻��̉���
	//m_physics->DrawColRange(m_actors);
#endif
}

ActorList_t ActorController::SearchCanLockOnActor() const
{
	ActorList_t result;

	for (auto& actor : m_actors)
	{
		if (!actor->CanLockOn()) continue;

		result.emplace_back(actor);
	}

	return result;
}

std::shared_ptr<Actor> ActorController::SearchNearestLockOnActor(const Vector3& pos) const
{
	auto actors = SearchCanLockOnActor();

	// �������Ȃ�����
	if (actors.empty()) return nullptr;

	std::shared_ptr<Actor> nearestActor = actors.front();
	float nearestLength = (nearestActor->GetPos() - pos).SqrMagnitude();

	for (auto& lockOnActor : actors)
	{
		auto length = (lockOnActor->GetPos() - pos).SqrMagnitude();

		if (length < nearestLength)
		{
			nearestActor = lockOnActor;
			nearestLength = length;
		}
	}

	return nearestActor;
}

ActorList_t ActorController::SearchEnemy() const
{
	ActorList_t result;
	for (auto& actor : m_actors)
	{
		if (actor->GetKind() != ActorKind::kEnemy) continue;
		result.emplace_back(actor);
	}
	return result;
}