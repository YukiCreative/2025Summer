#pragma once
#include <memory>
#include <list>
#include <string>

class Player;
class ActorController;
class UIController;

// プレイヤーにゲームの遊び方を教えます
class TutorialMode
{
public:
	TutorialMode();

	void Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController> actors, std::weak_ptr<UIController> ui);

	void Update();

private:

	std::weak_ptr<Player> m_player;
	std::weak_ptr<ActorController> m_actors;
	std::weak_ptr<UIController> m_UI;

	// チュートリアルテキスト
	std::list<std::string> m_tutoMessages;
};

