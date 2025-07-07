#include "EnemyGenerator.h"
#include "NoCollidable.h"

#include "EnemyTest.h"

EnemyGenerator::EnemyGenerator() :
	Actor(false)
{
}

void EnemyGenerator::Init()
{
	// 物理を設定しないといけないのは欠陥では？
	m_collidable = std::make_shared<NoCollidable>();
}

void EnemyGenerator::SpawnWave(const int waveNum)
{
	if (waveNum < 0 || waveNum > m_waveData.size()) return;

	auto& wave = m_waveData[waveNum];

	for (auto& data : wave)
	{
		// スポーン
		
		std::shared_ptr<Actor> spawnEnemy;

		// ここは愚直に全敵を把握して分岐するか
		switch (data.enemyKind)
		{
		case EnemyKind::kBug:
			auto a = std::make_shared<EnemyTest>();
			a->Init(data.pos);
			spawnEnemy = a;
			break;
		}

		SpawnActor(spawnEnemy);
	}
}