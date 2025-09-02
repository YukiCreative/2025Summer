#include "ScoreManager.h"
#include <array>

ScoreManager::ScoreManager() :
	m_score(0),
	m_wave(0)
{
	// 最初の要素を追加
	m_waveResultList.emplace_back(std::make_shared<WaveResult>());
}

ScoreManager& ScoreManager::GetInstance()
{
	static ScoreManager instance;
	return instance;
}

WaveRank ScoreManager::CalcWaveDamageRank(const int waveNum) const
{
	if (m_waveResultList.size() < waveNum) return WaveRank::kNoRank;

	// ダメージを受けるほどランクが下がる
	const float damage = m_waveResultList[waveNum]->m_damageAmount;

	return WaveRank::kSRank;
}

WaveRank ScoreManager::CalcWaveTimeRank(const int waveNum) const
{	
	if (m_waveResultList.size() < waveNum) return WaveRank::kNoRank;
	// クリアタイムが短いほどランクが上がる
	return WaveRank::kSRank;
}

WaveRank ScoreManager::CalcWaveStylishRank(const int waveNum) const
{
	if (m_waveResultList.size() < waveNum) return WaveRank::kNoRank;
	// スタイリッシュポイントが多いほどランクが上がる
	return WaveRank::kSRank;
}

WaveRank ScoreManager::CalcWaveOverallRank(const int waveNum) const
{
	// 三つのランクの平均
	std::array<WaveRank, 3> allRanks{CalcWaveDamageRank(waveNum), CalcWaveTimeRank(waveNum), CalcWaveStylishRank(waveNum)};
	WaveRank result = static_cast<WaveRank>(static_cast<int>(static_cast<float>(static_cast<int>(allRanks[0]) + static_cast<int>(allRanks[1]), static_cast<int>(allRanks[2])) / 3.0f));
	return result;
}

void ScoreManager::AddDamageAmount(const float damageAmount)
{
	m_waveResultList[m_wave]->m_damageAmount += damageAmount;
}

void ScoreManager::SetClearTime(const float clearTime)
{
	m_waveResultList[m_wave]->m_clearTime = clearTime;
}

void ScoreManager::AddStylishPoint(const float point)
{
	m_waveResultList[m_wave]->m_stylishRankPoint += point;
}

std::shared_ptr<WaveResult> ScoreManager::GetWaveResult(const int waveNum)
{
	if (m_waveResultList.size() < waveNum) return nullptr;

	return m_waveResultList[waveNum];
}

std::shared_ptr<WaveResult> ScoreManager::GetNowWaveResult()
{
	return m_waveResultList[m_wave];
}

std::vector<std::shared_ptr<WaveResult>> ScoreManager::GetAllWaveResult()
{
	return m_waveResultList;
}

WaveRank ScoreManager::CalcAllWaveOverallResult() const
{
	std::vector<WaveRank> ranks = CalcAllWaveRank();

	int sumRank = 0;

	for (auto& rank : ranks)
	{
		sumRank += static_cast<int>(rank);
	}

	return static_cast<WaveRank>(static_cast<float>(sumRank) / static_cast<float>(ranks.size()));
}

std::vector<WaveRank> ScoreManager::CalcAllWaveRank() const
{
	std::vector<WaveRank> result;

	for (int i = 0; i < m_waveResultList.size(); ++i)
	{
		result.emplace_back(CalcWaveOverallRank(i));
	}

	return result;
}

void ScoreManager::ProceedWave()
{
	++m_wave;
	m_waveResultList.push_back(std::make_shared<WaveResult>());
}
