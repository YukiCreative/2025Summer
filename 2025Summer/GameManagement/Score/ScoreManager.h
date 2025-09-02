#pragma once
#include <memory>
#include <vector>

class StylishRank;

enum class WaveRank
{
	kSRank,
	kARank,
	kBRank,
	kCRank,
	kDRank,
	kNoRank,
};

struct WaveResult
{
	// ウェーブのプレイ結果
	float m_damageAmount;
	float m_clearTime;
	float m_stylishRankPoint;
};

class ScoreManager
{
private:
	ScoreManager();
	ScoreManager(const ScoreManager&) = delete;
	void operator=(const ScoreManager&) = delete;

public:
	static ScoreManager& GetInstance();

	// 指定したウェーブのランクを計算して出します
	WaveRank CalcWaveDamageRank(const int waveNum) const;
	WaveRank CalcWaveTimeRank(const int waveNum) const;
	WaveRank CalcWaveStylishRank(const int waveNum) const;
	WaveRank CalcWaveOverallRank(const int waveNum) const;

	void AddDamageAmount(const float damageAmount);
	void SetClearTime(const float clearTime);
	void AddStylishPoint(const float point);

	std::shared_ptr<WaveResult> GetWaveResult(const int waveNum);
	std::shared_ptr<WaveResult> GetNowWaveResult();
	std::vector<std::shared_ptr<WaveResult>> GetAllWaveResult();
	
	WaveRank CalcAllWaveOverallResult() const;
	std::vector<WaveRank> CalcAllWaveRank() const;

	// ウェーブを進める
	// 今まで設定したスコアを1ウェーブの結果として保存する
	// 次のスコア追加関数は別の変数に記録される
	void ProceedWave();
private:

	float m_score;
	int m_wave;

	std::vector<std::shared_ptr<WaveResult>> m_waveResultList;
};

