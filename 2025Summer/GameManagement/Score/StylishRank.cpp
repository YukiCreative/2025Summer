#include "StylishRank.h"
#include "Game.h"
#include <cassert>

namespace
{
	constexpr size_t kMaxHistory = 10;

	// 位置フレームに自然減少する最大速度
	constexpr float kMaxDecreaseSpeed = 5.0f / Game::kFrameRate;
	// 自然減少加速度
	constexpr float kContinueDecreaseAccel = 0.0003f;

	// ここまで達したらそのランク
	constexpr float kDRankThreshold = 10.0f;
	constexpr float kCRankThreshold = 20.0f;
	constexpr float kBRankThreshold = 30.0f;
	constexpr float kARankThreshold = 40.0f;
	constexpr float kSRankThreshold = 50.0f;
	constexpr float kSSRankThreshold = 70.0f;
	constexpr float kSSSRankThreshold = 90.0f;
}

StylishRank& StylishRank::GetInstance()
{
	static StylishRank instance;
	return instance;
}

StylishRank::StylishRank() :
	m_spContinueDecreaseSpeed(0.0f)
{
}

void StylishRank::Init()
{
	InitMap();
}

void StylishRank::Update()
{
	// 時間でスタイリッシュポイントを減少させる
	// 増加していない時間が長いほど減少量を増やす
	// 増加したら時間リセット
	m_sp -= m_spContinueDecreaseSpeed;

	m_spContinueDecreaseSpeed += kContinueDecreaseAccel;
	if (m_spContinueDecreaseSpeed > kMaxDecreaseSpeed) m_spContinueDecreaseSpeed = kMaxDecreaseSpeed;
}

void StylishRank::IncreaseStylishPoint(const IncreaseStylishPointKind kind)
{
	assert(m_increasePointMap.contains(kind) && "対応してる列挙体の要素がMapになかったよ");

	// 連続して同じ方法で加点されてたら加点量を減らす
	const int historyCount = std::count(m_stylishActionHistory.begin(), m_stylishActionHistory.end(), kind);

	// この計算式なら一回目は100％二回目は50％三回目は33％…になっていく
	// 後で変えるかも
	m_sp += m_increasePointMap[kind] / (historyCount + 1);

	if (m_stylishActionHistory.size() > kMaxHistory)
	{
		m_stylishActionHistory.pop_front();
	}
	m_stylishActionHistory.emplace_back(kind);

	// スタイリッシュゲージ自然減少速度をリセット
	m_spContinueDecreaseSpeed = 0.0f;
}

void StylishRank::DecreaseStylishPoint(const DecreaseStylishPointKind kind)
{
	assert(m_decreasePointMap.contains(kind) && "対応してる列挙体の要素がMapになかったよ");
	m_sp -= m_decreasePointMap[kind];
}

const StylishRankUIParam StylishRank::GetRankUIParam() const
{
	StylishRankUIParam result;

	// 0~100のうち、どれくらいかでD~SSSまで分ける
	if		(m_sp.Value() < kDRankThreshold)	result.m_rank = StylishRankKind::kNoRank;
	else if (m_sp.Value() < kCRankThreshold)	result.m_rank = StylishRankKind::kDRank;
	else if (m_sp.Value() < kBRankThreshold)	result.m_rank = StylishRankKind::kCRank;
	else if (m_sp.Value() < kARankThreshold)	result.m_rank = StylishRankKind::kBRank;
	else if (m_sp.Value() < kSRankThreshold)	result.m_rank = StylishRankKind::kARank;
	else if (m_sp.Value() < kSSRankThreshold)	result.m_rank = StylishRankKind::kSRank;
	else if (m_sp.Value() < kSSSRankThreshold)	result.m_rank = StylishRankKind::kSSRank;
	else										result.m_rank = StylishRankKind::kSSSRank;

	// そのランクから次のランク(もしくは上限)までどれくらいの割合かを出して返す
	switch (result.m_rank)
	{
	case StylishRankKind::kNoRank:
		result.m_ratio = m_sp.Value() / kDRankThreshold;
		break;
	case StylishRankKind::kDRank:
		result.m_ratio = (m_sp.Value() - kDRankThreshold) / (kCRankThreshold - kDRankThreshold);
		break;
	case StylishRankKind::kCRank:
		result.m_ratio = (m_sp.Value() - kCRankThreshold) / (kBRankThreshold - kCRankThreshold);
		break;
	case StylishRankKind::kBRank:
		result.m_ratio = (m_sp.Value() - kBRankThreshold) / (kARankThreshold - kBRankThreshold);
		break;
	case StylishRankKind::kARank:
		result.m_ratio = (m_sp.Value() - kARankThreshold) / (kSRankThreshold - kARankThreshold);
		break;
	case StylishRankKind::kSRank:
		result.m_ratio = (m_sp.Value() - kSRankThreshold) / (kSSRankThreshold - kSRankThreshold);
		break;
	case StylishRankKind::kSSRank:
		result.m_ratio = (m_sp.Value() - kSSRankThreshold) / (kSSSRankThreshold - kSSRankThreshold);
		break;
	case StylishRankKind::kSSSRank:
		result.m_ratio = (m_sp.Value() - kSSSRankThreshold) / (StylishRankRange::kRankMax - kSSSRankThreshold);
		break;
	default:
		assert(false && "StylishKindのswitch文漏れ");
		break;
	}

	return result;
}

void StylishRank::ResetRank()
{
	m_sp.SetMin();
	m_stylishActionHistory.clear();
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
