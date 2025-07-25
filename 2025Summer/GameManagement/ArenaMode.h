#pragma once
#include <memory>

class Player;
class ActorController;
class WaveController;
class Timer;
class UIController;

class ArenaMode
{
public:
	ArenaMode();

	void Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController> actors, std::weak_ptr<UIController> ui);

	void Update();

private:

	std::shared_ptr<WaveController> m_wave;
	std::shared_ptr<Timer> m_timer;
	std::weak_ptr<ActorController> m_actors;
};

