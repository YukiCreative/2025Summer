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
		actor->Update();
		// ¶‚«‚Ä‚é‚â‚ÂƒŠƒXƒg‚ðì‚é
		if (actor->IsAlive())
		{
			aliveActors.emplace_back(actor);
		}
		// ‰½‚©Actor‚ð¶¬‚µ‚Ä‚¢‚½‚ç
		if (actor->IsSpawnedActor())
		{
			// ‚»‚ê‚ðE‚Á‚Ä‚±‚¿‚ç‚ÌActorList‚ÅŠÇ—‚·‚é
			aliveActors.sort();
			aliveActors.merge(actor->GetSpawnActor());
			actor->ClearActorList();
		}
	}
	// m_actors‚ð¶‚«‚Ä‚é‚â‚Â‚¾‚¯‚É‚·‚é
	m_actors = aliveActors;

	// d—Í‚Æ‚©“–‚½‚è”»’è‚Æ‚©
	m_physics->Update(m_actors);

	// C³Œã‚ÌˆÚ“®—Ê”½‰f
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
	// “–‚½‚è”»’è‚Ì‰ÂŽ‹‰»
	m_physics->DrawColRange(m_actors);
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
