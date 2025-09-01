#include "WaveResultUI.h"
#include <string>
#include "Image.h"
#include <DxLib.h>
#include "../General/Game.h"
#include "../GameManagement/Score/ScoreManager.h"
#include "../Geometry/Geometry.h"
#include <cmath>

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
	const Vector2 kDamageDrawOffset = {-100.0f, 0.0f};
	const Vector2 kTimeDrawOffset = {0.0f, 0.0f};
	const Vector2 kStylishPointDrawOffset = {100.0f, 0.0f};

	constexpr float kLerpSpeed = 0.02f;

	constexpr float kBoxWidth = 100.0f;
	constexpr float kBoxHeight = 100.0f;
}

WaveResultUI::WaveResultUI() :
	UIBase(ToString(WaveResultUI)),
	m_fontH(-1),
	m_sequence(&WaveResultUI::Disable),
	m_draw(&WaveResultUI::NoDraw),
	m_lerpStartPos()
{
}

WaveResultUI::~WaveResultUI()
{
	DeleteFontToHandle(m_fontH);
}

void WaveResultUI::Init()
{
	// フォント作る
	m_fontH = CreateFontToHandle(kFontName.c_str(), kFontSize, kFontThick);
	m_pos = kInitPos;
}

void WaveResultUI::Update()
{
	(this->*m_sequence)();
}

void WaveResultUI::Draw() const
{
	(this->*m_draw)();
}

void WaveResultUI::StartResult()
{
	m_pos = kInitPos;
	m_lerpStartPos = m_pos;
	m_lerpTime.SetMin();

	m_resultData = ScoreManager::GetInstance().GetNowWaveResult();

	m_sequence = &WaveResultUI::Appear;
	m_draw = &WaveResultUI::BoxDraw;
}

void WaveResultUI::Appear()
{
	m_lerpTime += kLerpSpeed;
	float temp = Geometry::Easing(m_lerpTime, Geometry::EasingKind::kOutQuart);
	// 出現
	m_pos = m_lerpStartPos + m_lerpStartPos.Lerp(kAppearTargetPos, Geometry::Easing(m_lerpTime, Geometry::EasingKind::kOutQuart));

	if (m_lerpTime.IsMax())
	{
		m_sequence = &WaveResultUI::CountDamageAmount;
		m_draw = &WaveResultUI::DamageDraw;
		m_lerpTime.SetMin();
		return;
	}
}

void WaveResultUI::CountDamageAmount()
{
	m_lerpTime += kLerpSpeed;

	if (m_lerpTime.IsMax())
	{
		m_lerpTime.SetMin();
		m_sequence = &WaveResultUI::CountClearTime;
	}
}

void WaveResultUI::CountClearTime()
{
	m_lerpTime += kLerpSpeed;

	if (m_lerpTime.IsMax())
	{
		m_lerpTime.SetMin();
		m_sequence = &WaveResultUI::CountTotalStylishPoint;
	}
}

void WaveResultUI::CountTotalStylishPoint()
{
	m_lerpTime += kLerpSpeed;

	if (m_lerpTime.IsMax())
	{
		m_lerpTime.SetMin();
		m_sequence = &WaveResultUI::CountTotalStylishPoint;
	}
}

void WaveResultUI::Disappear()
{
	// 戻る
	m_pos.LerpMyself(kInitPos, kLerpSpeed);
}

void WaveResultUI::Disable()
{
	// 何もしない
}

void WaveResultUI::BoxDraw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(m_pos.x - kBoxWidth, m_pos.y - kBoxHeight, m_pos.x + kBoxWidth, m_pos.y + kBoxHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void WaveResultUI::DamageDraw() const
{
	BoxDraw();
	// 数字出す
	DrawFormatStringToHandle(m_pos.x + kDamageDrawOffset.x, m_pos.y + kDamageDrawOffset.y, 0xffffff, m_fontH, "ダメージ：%f", std::lerp(0.0f, m_resultData.lock()->m_damageAmount, Geometry::Easing(m_lerpTime, Geometry::EasingKind::kOutQuart)));
}

void WaveResultUI::TimeDraw() const
{
	BoxDraw();
	DrawFormatStringToHandle(m_pos.x + kDamageDrawOffset.x, m_pos.y + kDamageDrawOffset.y, 0xffffff, m_fontH, "ダメージ　　：%f", m_resultData.lock()->m_damageAmount);
	DrawFormatStringToHandle(m_pos.x + kTimeDrawOffset.x, m_pos.y + kTimeDrawOffset.y, 0xffffff, m_fontH, "クリアタイム：%f", std::lerp(0.0f, m_resultData.lock()->m_clearTime, Geometry::Easing(m_lerpTime, Geometry::EasingKind::kOutQuart)));
}

void WaveResultUI::StylishPointDraw() const
{
	BoxDraw();
	DrawFormatStringToHandle(m_pos.x + kDamageDrawOffset.x, m_pos.y + kDamageDrawOffset.y, 0xffffff, m_fontH, "ダメージ　　：%f", m_resultData.lock()->m_damageAmount);
	DrawFormatStringToHandle(m_pos.x + kTimeDrawOffset.x, m_pos.y + kTimeDrawOffset.y, 0xffffff, m_fontH, "クリアタイム：%f", m_resultData.lock()->m_clearTime);
	DrawFormatStringToHandle(m_pos.x + kStylishPointDrawOffset.x, m_pos.y + kStylishPointDrawOffset.y, 0xffffff, m_fontH, "スタイリッシュポイント：%f", std::lerp(0.0f, m_resultData.lock()->m_stylishRankPoint, Geometry::Easing(m_lerpTime, Geometry::EasingKind::kOutQuart)));
}

void WaveResultUI::NoDraw() const
{
}
