#include "ScoreManager.h"

ScoreManager::ScoreManager()
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
