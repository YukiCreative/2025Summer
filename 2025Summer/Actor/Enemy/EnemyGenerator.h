#pragma once
#include <list>
#include <vector>
#include "Vector3.h"
#include "EnemyKind.h"
#include "Actor.h"

// �G�̔������Ǘ�����
// Scene������
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

	// �w��̃E�F�[�u�̓G�𔭐�������
	void SpawnWave(const int waveNum);

private:

	using WaveData_t = std::vector<std::list<SpawnData>>;
	WaveData_t m_waveData;
};

