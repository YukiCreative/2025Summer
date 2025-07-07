#include "EnemyGenerator.h"
#include "NoCollidable.h"

#include "EnemyTest.h"

EnemyGenerator::EnemyGenerator() :
	Actor(false)
{
}

void EnemyGenerator::Init()
{
	// ������ݒ肵�Ȃ��Ƃ����Ȃ��̂͌��ׂł́H
	m_collidable = std::make_shared<NoCollidable>();
}

void EnemyGenerator::SpawnWave(const int waveNum)
{
	if (waveNum < 0 || waveNum > m_waveData.size()) return;

	auto& wave = m_waveData[waveNum];

	for (auto& data : wave)
	{
		// �X�|�[��
		
		std::shared_ptr<Actor> spawnEnemy;

		// �����͋𒼂ɑS�G��c�����ĕ��򂷂邩
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