#pragma once
#include <memory>

class EnemyGenerator;

class Player;
class ActorController;

// “G‚ÌoŒ»‚ğ§Œä‚·‚é
class WaveController
{
public:
	WaveController();

	void Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController> actors);
	void Update();

	void StopUpdate() { m_state = &WaveController::InActiveUpdate; }

private:

	std::shared_ptr<EnemyGenerator> m_enemyGenerator;
	std::weak_ptr<ActorController> m_actors;
	int m_wave;
	int m_stateFrame;

	using WaveState_t = void (WaveController::*)();
	WaveState_t m_state;

private:

	// “G‚ªoŒ»‚µ‚Ä‚¢‚éŠÔ
	void BattleUpdate();
	// “G‚ğ‘S•”“|‚µ‚Ä‘Ò‹@‚µ‚Ä‚¢‚é‚Æ‚«
	void IntervalUpdate();
	// ‰½‚à‚µ‚È‚¢
	void InActiveUpdate() {}
};

