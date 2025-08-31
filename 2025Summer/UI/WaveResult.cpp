#include "WaveResult.h"
#include <string>
#include "Image.h"
#include <DxLib.h>
#include "../General/Game.h"
#include "../GameManagement/Score/ScoreManager.h"

namespace
{
	const std::string kFontName = "";
	constexpr int kFontSize = 10;
	constexpr int kFontThick = 1;

	constexpr int kAppearFrame = 60;
	constexpr int kLifeFrame = 180;
	constexpr int kDisappearFrame = 60;

	const Vector2 kInitPos = { Game::kScreenWidth + 500.0f, 500.0f};
	const Vector2 kAppearTargetPos = { Game::kScreenWidth - 500.0f, 500.0f};
	constexpr float kLerpSpeed = 0.3f;
}

WaveResult::WaveResult() :
	UIBase(ToString(WaveResult)),
	m_fontH(-1),
	m_damageAmount(0),
	m_rank(WaveRank::kNoRank),
	m_sequence(&WaveResult::Appear)
{
}

WaveResult::~WaveResult()
{
	DeleteFontToHandle(m_fontH);
}

void WaveResult::Init()
{
	// フォント作る
	m_fontH = CreateFontToHandle(kFontName.c_str(), kFontSize, kFontThick);
}

void WaveResult::Update()
{
	(this->*m_sequence)();
}

void WaveResult::Draw() const
{
	m_image->Draw(m_pos);
}

void WaveResult::StartResult()
{
	m_pos = kInitPos;
	m_sequence = &WaveResult::Appear;
}

void WaveResult::Appear()
{
	// 出現
	m_pos.LerpMyself(kAppearTargetPos, kLerpSpeed);
}

void WaveResult::Wait()
{
	// 数字出す

}

void WaveResult::Disappear()
{
	// 戻る
	m_pos.LerpMyself(kInitPos, kLerpSpeed);
}