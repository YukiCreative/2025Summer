#pragma once
#include <memory>

class Player;
class ActorController;
class WaveController;
class Timer;

class ArenaMode
{
public:
	ArenaMode();

	void Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController> actors);

	void Update();

private:

	std::shared_ptr<WaveController> m_wave;
	std::shared_ptr<Timer> m_timer;
};

