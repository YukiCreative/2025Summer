#include "EnemyBug.h"
#include "EnemyGenerator.h"
#include "EnemyPlant.h"
#include "NoCollidable.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	constexpr int kWaveNum = 1;
	const std::string kWaveBasePath = "Data/WaveData/Wave";
	constexpr float kPosMult = 100.0f;
}

EnemyGenerator::EnemyGenerator() :
	Actor(false)
{
}

void EnemyGenerator::Init(std::weak_ptr<Player> player)
{
	m_player = player;

	// 物理を設定しないといけないのは欠陥では？
	m_collidable = std::make_shared<NoCollidable>();

	InitFactory();

	LoadWaveData();
}

void EnemyGenerator::SpawnWave(const int waveNum)
{
	if (waveNum < 0 || waveNum > m_waveData.size()) return;

	auto& wave = m_waveData[waveNum];

	for (auto& data : wave)
	{
		// スポーン

		SpawnActor(m_factory[data.enemyName](m_player, data.pos * kPosMult));
	}
}

void EnemyGenerator::InitFactory()
{
	m_factory["bug"] = [](std::weak_ptr<Player> player, const Vector3& initPos)->std::shared_ptr<Actor>
	{
		auto enemy = std::make_shared<EnemyBug>();
		enemy->Init(player, initPos);
		return enemy;
	};
	m_factory["Plant"] = [](std::weak_ptr<Player> player, const Vector3& initPos)->std::shared_ptr<Actor>
	{
		auto enemy = std::make_shared<EnemyPlant>();
		enemy->Init(player, initPos);
		return enemy;
	};
}

void EnemyGenerator::LoadWaveData()
{
	for (int wave = 0; wave < kWaveNum; ++wave)
	{
		std::vector<SpawnData> waveData;

		const std::string nowWavePath = kWaveBasePath + std::to_string(wave) + ".dat";

		int waveH = FileRead_open(nowWavePath.c_str());

		if (waveH == 0)
		{
			assert(false && "Waveデータがない");
			return;
		}

		int enemyNum;

		// このウェーブの敵の数を取得
		FileRead_read(&enemyNum, sizeof(enemyNum), waveH);

		for (int enemy = 0; enemy < enemyNum; ++enemy)
		{
			// 名前を取得
			char enemyNameLength;
			FileRead_read(&enemyNameLength, sizeof(enemyNameLength), waveH);

			std::string enemyName;
			enemyName.resize(enemyNameLength);

			FileRead_read(enemyName.data(), enemyName.size(), waveH);

			// 名前から種類を特定

			// 次に位置を取得
			Vector3 enemyPos;

			FileRead_read(&enemyPos, sizeof(enemyPos), waveH);

			// 取得したデータをまとめる
			waveData.emplace_back(SpawnData(enemyName, enemyPos));
		}

		FileRead_close(waveH);

		m_waveData.emplace_back(waveData);
	}
}
