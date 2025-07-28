#pragma once
#include "RangeLimitedValue.h"
#include <unordered_map>
#include <list>

namespace StylishRankRange
{
	constexpr float kRankMin = 0.0f;
	constexpr float kRankMax = 100.0f;
}

// 加点行動
enum class IncreaseStylishPointKind
{
	kSlashDown,
	kSlashLeftSide,
	kSlashRightSide,
	kSlashUp,
	kFinishAttack,
	kChargeAttack,
	kShockWaveAttack,
	kShockWave,
	kSpecialAttack,
};

// 減点行動
enum class DecreaseStylishPointKind
{
	kDamage,
};

enum class StylishRankKind
{
	kNoRank, // 初期状態　ランクの表示なし
	kDRank,
	kCRank,
	kBRank,
	kARank,
	kSRank,
	kSSRank,
	kSSSRank
};

struct StylishRankUIParam
{
	StylishRankKind m_rank = StylishRankKind::kNoRank;
	RangeLimitedValue<float, 0.0f, 1.0f> m_ratio;
};

class StylishRank
{
private:
	StylishRank();
	StylishRank(const StylishRank&) = delete;
	void operator=(const StylishRank&) = delete;

public:
	static StylishRank& GetInstance();

	void Init();

	void Update();

	// この二つは計算方式が違うだけなのでIncreaseとかDecreaseとかに列挙を分ける必要はないかもしれない

	// 攻撃が当たったらこれに攻撃手段を渡す
	// その攻撃の基礎値や連続して出してないかなどでポイントを付ける
	void IncreaseStylishPoint(const IncreaseStylishPointKind kind);
	// 加点と計算式が異なる　基本的に設定された値のまま減算される
	void DecreaseStylishPoint(const DecreaseStylishPointKind kind);

	// 0~100
	const float GetStylishPoint() const { return m_sp.Value(); }
	// 今のスタイリッシュランクとその割合を返す
	const StylishRankUIParam GetRankUIParam() const;

	void ResetRank();

private:

	using StylishPoint_t = RangeLimitedValue<float, StylishRankRange::kRankMin, StylishRankRange::kRankMax>;
	StylishPoint_t m_sp;

	std::unordered_map<IncreaseStylishPointKind, float> m_increasePointMap;
	std::unordered_map<DecreaseStylishPointKind, float> m_decreasePointMap;

	std::list<IncreaseStylishPointKind> m_stylishActionHistory;

	float m_spContinueDecreaseSpeed;

private:

	void InitMap();
};

