#include "TutorialMode.h"

TutorialMode::TutorialMode()
{
}

void TutorialMode::Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController> actors, std::weak_ptr<UIController> ui)
{
	m_player = player;
	m_actors = actors;
	m_UI = ui;
}

void TutorialMode::Update()
{
	// ƒvƒŒƒCƒ„[‚Ì“ü—Í‚ğŒ©‚Ü‚·

}
