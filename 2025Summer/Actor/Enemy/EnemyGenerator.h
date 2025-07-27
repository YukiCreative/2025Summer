#pragma once
#include <string>
#include "EnemyKind.h"
#include "Vector3.h"
#include <unordered_map>
#include <vector>

class Player;
class Enemy;
class EnemyModelList;
class ActorController;
class Actor;

// �G�̔������Ǘ�����
// Scene������
class EnemyGenerator
{
	struct SpawnData
	{
		std::string enemyName;
		Vector3 pos;


		SpawnData(const std::string& name, const Vector3& _pos) : pos(_pos), enemyName(name) {}
	};

public:
	EnemyGenerator();
	~EnemyGenerator();

	void Init(std::weak_ptr<Player> player, std::weak_ptr<ActorController>);

	// �w��̃E�F�[�u�̓G�𔭐�������
	void SpawnWave(const int waveNum);

private:

	using WaveData_t = std::vector<std::vector<SpawnData>>;
	WaveData_t m_waveData;

	using EnemyFactoryFunction_t = std::shared_ptr<Enemy> (*)(std::weak_ptr<Player>, const Vector3&, std::shared_ptr<EnemyModelList>);

	// ����������ēG�𐶐�����֐���Ԃ�
	using EnemyFactory_t = std::unordered_map<std::string, EnemyFactoryFunction_t>;
	EnemyFactory_t m_factory;
	std::shared_ptr<EnemyModelList> m_handles;

	std::weak_ptr<Player> m_player;
	std::weak_ptr<ActorController> m_actors;

private:

	// �����̂ŕ�����
	void InitFactory();
	void LoadWaveData();
	void LoadModelData();

	// �t�B�[���h�����ɉ�]������
	void RotateOriginPos(std::shared_ptr<Enemy> spawnEnemy);
};

