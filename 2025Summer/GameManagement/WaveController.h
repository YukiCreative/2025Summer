#pragma once
#include <memory>

class EnemyGenerator;

class Player;
class ActorController;

// �G�̏o���𐧌䂷��
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

	// �G���o�����Ă����
	void BattleUpdate();
	// �G��S���|���đҋ@���Ă���Ƃ�
	void IntervalUpdate();
	// �������Ȃ�
	void InActiveUpdate() {}
};

