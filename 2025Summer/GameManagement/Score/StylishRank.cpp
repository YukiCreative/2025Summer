#include "StylishRank.h"
#include <cassert>

namespace
{
	constexpr size_t kMaxHistory = 10;
}

StylishRank::StylishRank()
{
}

void StylishRank::Init()
{
	InitMap();
}

void StylishRank::Update()
{
	// やりたいこと
	// 時間でスタイリッシュポイントを減少させたい
	// 増加していない時間が長いほど減少量を増やす
	// 増加したら時間リセット
	// 減少速度には最大値がある
}

void StylishRank::IncreaseStylishPoint(const IncreaseStylishPointKind kind)
{
	assert(m_increasePointMap.contains(kind) && "対応してる列挙体の要素がMapになかったよ");

	// やりたいこと
	// 連続して同じ方法で加点されてたら加点量を減らす
	// そのためにどんな加点方法を記憶する
	// 減衰効果をどれくらい持続させるかはまだ考え中(リアルタイムでいいのか、行動しないといけないのか)
	// また、過去何件中に存在しなかった加点方法はボーナスとかあってもいい

	m_sp += m_increasePointMap[kind];

	if (m_stylishActionHistory.size() > kMaxHistory)
	{
		m_stylishActionHistory.pop_front();
	}
	m_stylishActionHistory.emplace_back(kind);
}

void StylishRank::DecreaseStylishPoint(const DecreaseStylishPointKind kind)
{
	assert(m_decreasePointMap.contains(kind) && "対応してる列挙体の要素がMapになかったよ");
	m_sp -= m_decreasePointMap[kind];
}

void StylishRank::InitMap()
{
	// ひとまず定数
	// あとで外部化するかも
	m_increasePointMap[IncreaseStylishPointKind::kSlashDown] = 2.0f;
	m_increasePointMap[IncreaseStylishPointKind::kSlashLeftSide] = 2.0f;
	m_increasePointMap[IncreaseStylishPointKind::kSlashRightSide] = 5.0f;
	m_increasePointMap[IncreaseStylishPointKind::kSlashUp] = 5.0f;
	m_increasePointMap[IncreaseStylishPointKind::kFinishAttack] = 15.0f;
	m_increasePointMap[IncreaseStylishPointKind::kChargeAttack] = 8.0f;
	m_increasePointMap[IncreaseStylishPointKind::kShockWaveAttack] = 3.0f;
	m_increasePointMap[IncreaseStylishPointKind::kShockWave] = 8.0f;
	m_increasePointMap[IncreaseStylishPointKind::kSpecialAttack] = 20.0f;

	m_decreasePointMap[DecreaseStylishPointKind::kDamage] = 40.0f;
}
