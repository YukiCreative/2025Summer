#include "ScoreManager.h"

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

WaveRank ScoreManager::CalcWaveDamageRank(const int waveNum)
{
	if (m_waveResultList.size() < waveNum) return WaveRank::kNoRank;

	// ダメージを受けるほどランクが下がる
	const float damage = m_waveResultList[waveNum]->m_damageAmount;

	return WaveRank::kSRank;
}

WaveRank ScoreManager::CalcWaveTimeRank(const int waveNum)
{	
	if (m_waveResultList.size() < waveNum) return WaveRank::kNoRank;
	// クリアタイムが短いほどランクが上がる
	return WaveRank::kSRank;
}

WaveRank ScoreManager::CalcWaveStylishRank(const int waveNum)
{
	if (m_waveResultList.size() < waveNum) return WaveRank::kNoRank;
	// スタイリッシュポイントが多いほどランクが上がる
	return WaveRank::kSRank;
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

void ScoreManager::ProceedWave()
{
	++m_wave;
	m_waveResultList.push_back(std::make_shared<WaveResult>());
}
