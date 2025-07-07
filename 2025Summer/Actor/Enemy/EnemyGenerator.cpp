#include "EnemyGenerator.h"
#include "NoCollidable.h"

#include "EnemyBug.h"

EnemyGenerator::EnemyGenerator() :
	Actor(false)
{
}

void EnemyGenerator::Init(std::weak_ptr<Player> player)
{
	m_player = player;

	// 物理を設定しないといけないのは欠陥では？
	m_collidable = std::make_shared<NoCollidable>();

	// TODO:何らかの方法でウェーブの情報を取得する
	// とりあえずベタ書きでもOK
	std::vector<SpawnData> temp;
	temp.emplace_back(SpawnData({150.0f,0,150.0f }, EnemyKind::kBug));

	m_waveData.emplace_back(temp);
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
			auto a = std::make_shared<EnemyBug>();
			a->Init(m_player, data.pos);
			spawnEnemy = a;
			break;
		}

		SpawnActor(spawnEnemy);
	}
}