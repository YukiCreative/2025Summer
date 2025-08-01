#include "Actor.h"
#include "ActorController.h"
#include "../Physics/Physics.h"
#include <DxLib.h>
#include <list>
#include "Enemy/Enemy.h"

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
		// もし停止中ならUpdateしない
		if (!actor->IsStop())
		{
			actor->Update();
		}
		actor->LimitMovementRange();

		// 生きてるやつリストを作る
		if (actor->IsAlive())
		{
			aliveActors.emplace_back(actor);
		}
		else if (actor->GetKind() == ActorKind::kEnemy)
		{
			// 敵が死んだとき、死んだ敵を記録
			auto enemy = std::static_pointer_cast<Enemy>(actor);

			m_beforeDeathEnemy.emplace_back(enemy->GetEnemyKind());
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
	// 当たり判定の可視化
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