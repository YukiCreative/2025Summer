#pragma once
#include <memory>

class EnemyGenerator;

class Player;
class ActorController;

// 敵の出現を制御する
class WaveController
{
public:
	WaveController();

	void Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController> actors);
	void Update();

	void StopUpdate() { m_state = &WaveController::InActiveUpdate; }

	// ウェーブが切り替わった(敵が全滅した)フレームだけtrueになります
	const bool IsDefeatedAllEnemy() const { return m_isDefeatedAllEnemy; }
	const int GetNowWave() const { return m_wave; }

private:

	std::shared_ptr<EnemyGenerator> m_enemyGenerator;
	std::weak_ptr<ActorController> m_actors;
	int m_wave;
	int m_stateFrame;
	bool m_isDefeatedAllEnemy;

	using WaveState_t = void (WaveController::*)();
	WaveState_t m_state;

private:

	// 敵が出現している間
	void BattleUpdate();
	// 敵を全部倒して待機しているとき
	void IntervalUpdate();
	// 何もしない
	void InActiveUpdate() {}
};

