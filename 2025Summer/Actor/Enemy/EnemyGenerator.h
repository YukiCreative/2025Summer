#pragma once
#include "Actor.h"
#include "EnemyKind.h"
#include "Vector3.h"
#include <unordered_map>
#include <vector>

class Player;
class Enemy;

// 敵の発生を管理する
// Sceneが持つ
class EnemyGenerator : public Actor
{
	struct SpawnData
	{
		std::string enemyName;
		Vector3 pos;


		SpawnData(const std::string& name, const Vector3& _pos) : pos(_pos), enemyName(name) {}
	};

public:
	EnemyGenerator();

	void Init(std::weak_ptr<Player> player);

	void Update() override {}

	void Draw() const override {}

	void CommitMove() override {}

	// 指定のウェーブの敵を発生させる
	void SpawnWave(const int waveNum);

private:

	using WaveData_t = std::vector<std::vector<SpawnData>>;
	WaveData_t m_waveData;

	using EnemyFactoryFunction_t = std::shared_ptr<Actor> (*)(std::weak_ptr<Player>, const Vector3&);

	// 文字列を見て敵を返す
	using EnemyFactory_t = std::unordered_map<std::string, EnemyFactoryFunction_t>;
	EnemyFactory_t m_factory;

	std::weak_ptr<Player> m_player;

private:

	// 長いので分ける
	void InitFactory();
	void LoadWaveData();
};

