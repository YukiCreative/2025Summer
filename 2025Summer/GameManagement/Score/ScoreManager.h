#pragma once
#include <memory>

class StylishRank;

class ScoreManager
{
private:
	ScoreManager();
	ScoreManager(const ScoreManager&) = delete;
	void operator=(const ScoreManager&) = delete;

public:
	static ScoreManager& GetInsatnce();

	float GetScore() const;
	void AddScore(const float add);

private:

	float m_score;
};

