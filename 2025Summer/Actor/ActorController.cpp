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
		// ‚à‚µ’â~’†‚È‚çUpdate‚µ‚È‚¢
		if (!actor->IsStop())
		{
			actor->Update();
		}
		actor->LimitMovementRange();

		// ¶‚«‚Ä‚é‚â‚ÂƒŠƒXƒg‚ğì‚é
		if (actor->IsAlive())
		{
			aliveActors.emplace_back(actor);
		}
		else if (actor->GetKind() == ActorKind::kEnemy)
		{
			// “G‚ª€‚ñ‚¾‚Æ‚«A€‚ñ‚¾“G‚ğ‹L˜^
			auto enemy = std::static_pointer_cast<Enemy>(actor);

			m_beforeDeathEnemy.emplace_back(enemy->GetEnemyKind());
		}

		// ‰½‚©Actor‚ğ¶¬‚µ‚Ä‚¢‚½‚ç
		if (actor->IsSpawnedActor())
		{
			// ‚»‚ê‚ğE‚Á‚Ä‚±‚¿‚ç‚ÌActorList‚ÅŠÇ—‚·‚é
			aliveActors.sort();
			aliveActors.merge(actor->GetSpawnActor());
			actor->ClearActorList();
		}
	}
	// m_actors‚ğ¶‚«‚Ä‚é‚â‚Â‚¾‚¯‚É‚·‚é
	m_actors = aliveActors;

	// d—Í‚Æ‚©“–‚½‚è”»’è‚Æ‚©
	m_physics->Update(m_actors);

	// C³Œã‚ÌˆÚ“®—Ê”½‰f
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
	// “–‚½‚è”»’è‚Ì‰Â‹‰»
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

	// ‰½‚à‚¢‚È‚©‚Á‚½
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