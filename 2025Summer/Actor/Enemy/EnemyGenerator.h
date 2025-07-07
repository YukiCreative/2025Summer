#pragma once
#include <list>
#include <vector>
#include "Vector3.h"
#include "EnemyKind.h"
#include "Actor.h"

// 敵の発生を管理する
// Sceneが持つ
class EnemyGenerator : public Actor
{
	struct SpawnData
	{
		Vector3 pos;
		EnemyKind enemyKind;
	};

public:
	EnemyGenerator();

	void Init();

	void Update() override {}

	void Draw() const override {}

	void CommitMove() override {}

	// 指定のウェーブの敵を発生させる
	void SpawnWave(const int waveNum);

private:

	using WaveData_t = std::vector<std::list<SpawnData>>;
	WaveData_t m_waveData;
};

