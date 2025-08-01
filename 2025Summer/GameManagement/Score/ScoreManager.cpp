#include "ScoreManager.h"

ScoreManager::ScoreManager() :
	m_score(0)
{
}

ScoreManager& ScoreManager::GetInsatnce()
{
	static ScoreManager instance;
	return instance;
}

float ScoreManager::GetScore() const
{
	return m_score;
}

void ScoreManager::AddScore(const float add)
{
	m_score += add;
}
