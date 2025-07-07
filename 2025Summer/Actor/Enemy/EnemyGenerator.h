#pragma once
#include <vector>
#include "Vector3.h"
#include "EnemyKind.h"
#include "Actor.h"

class Player;

// �G�̔������Ǘ�����
// Scene������
class EnemyGenerator : public Actor
{
	struct SpawnData
	{
		Vector3 pos;
		EnemyKind enemyKind;

		SpawnData(const Vector3& _pos, const EnemyKind& kind) : pos(_pos), enemyKind(kind) {}
	};

public:
	EnemyGenerator();

	void Init(std::weak_ptr<Player> player);

	void Update() override {}

	void Draw() const override {}

	void CommitMove() override {}

	// �w��̃E�F�[�u�̓G�𔭐�������
	void SpawnWave(const int waveNum);

private:

	using WaveData_t = std::vector<std::vector<SpawnData>>;
	WaveData_t m_waveData;

	std::weak_ptr<Player> m_player;
};

