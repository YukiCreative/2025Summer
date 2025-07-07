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

	// ������ݒ肵�Ȃ��Ƃ����Ȃ��̂͌��ׂł́H
	m_collidable = std::make_shared<NoCollidable>();

	// TODO:���炩�̕��@�ŃE�F�[�u�̏����擾����
	// �Ƃ肠�����x�^�����ł�OK
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
		// �X�|�[��
		
		std::shared_ptr<Actor> spawnEnemy;

		// �����͋𒼂ɑS�G��c�����ĕ��򂷂邩
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