#include "Actor.h"
#include "ActorController.h"
#include "Physics.h"
#include <DxLib.h>
#include <list>

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
	ActorList_t aliveActors;
	for (auto& actor : m_actors)
	{
		actor->CountStopFrame();
		// もし停止中ならUpdateしない
		if (!actor->IsStopUpdate())
		{
			actor->Update();
		}
		actor->LimitMovementRange();

		// 生きてるやつリストを作る
		if (actor->IsAlive())
		{
			aliveActors.emplace_back(actor);
		}
		// 何かActorを生成していたら
		if (actor->IsSpawnedActor())
		{
			// それを拾ってこちらのActorListで管理する
			aliveActors.sort();
			aliveActors.merge(actor->GetSpawnActor());
			actor->ClearActorList();
		}
	}
	// m_actorsを生きてるやつだけにする
	m_actors = aliveActors;

	// 重力とか当たり判定とか
	m_physics->Update(m_actors);

	// 修正後の移動量反映
	for (auto& actor : m_actors)
	{
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
	// 当たり判定の可視化
	//m_physics->DrawColRange(m_actors);
#endif
}

bool ActorController::CheckIsAlive(std::shared_ptr<Actor> checkAct)
{
	for (const auto& actor : m_actors)
	{
		if (actor.get() == checkAct.get())
		{
			return true;
		}
	}
	return false;
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

std::shared_ptr<Actor> ActorController::GetNearestLockOnActor(const Vector3& pos) const
{
	auto actors = SearchCanLockOnActor();

	// 何もいなかった
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
