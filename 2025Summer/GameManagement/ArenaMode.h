#pragma once
#include <memory>
#include <unordered_map>
#include "EnemyKind.h"

class Player;
class ActorController;
class WaveController;
class Timer;
class UIController;
class Enemy;

class ArenaMode
{
public:
	ArenaMode();

	void Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController> actors, std::weak_ptr<UIController> ui);

	void Update();

private:

	// TODO:WaveControllerをArenaModeに統合する
	std::shared_ptr<WaveController> m_wave;
	std::shared_ptr<Timer> m_timer;
	std::weak_ptr<ActorController> m_actors;
	// 敵と、そのスコアの組み合わせ
	std::unordered_map<EnemyKind, int> m_addFrameMap;

};

