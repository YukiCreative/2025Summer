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
	const std::string kFontName = "BIZ UDP明朝 Medium";
	constexpr int kFontSize = 15;
	constexpr int kFontThick = 1;

	constexpr int kAppearFrame = 60;
	constexpr int kLifeFrame = 180;
	constexpr int kDisappearFrame = 60;

	const Vector2 kResultTextOffset = { -150.0f, -75.0f };
	const Vector2 kInitPos = { Game::kScreenWidth + 500.0f, 150.0f };
	const Vector2 kAppearTargetPos = { Game::kScreenWidth - 200.0f, 150.0f };
	const Vector2 kDamageDrawOffset = {-150.0f, -25.0f};
	const Vector2 kTimeDrawOffset = {-150.0f, 0.0f};
	const Vector2 kStylishPointDrawOffset = {-150.0f, 25.0f};

	constexpr float kLerpSpeed = 0.02f;

	constexpr float kBoxWidth = 150.0f;
	constexpr float kBoxHeight = 100.0f;

	constexpr int kShowWaitTime = 120;
}

WaveResultUI::WaveResultUI() :
	UIBase(ToString(WaveResultUI)),
	m_fontH(-1),
	m_sequence(&WaveResultUI::Disable),
	m_draw(&WaveResultUI::NoDraw),
	m_lerpStartPos(),
	m_frame(0)
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

	for (auto& img : m_rankImage)
	{
		img = std::make_shared<Image>();
		img->Init(-1);
	}
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
		m_draw = &WaveResultUI::TimeDraw;
		return;
	}
}

void WaveResultUI::CountClearTime()
{
	m_lerpTime += kLerpSpeed;

	if (m_lerpTime.IsMax())
	{
		m_lerpTime.SetMin();
		m_sequence = &WaveResultUI::CountTotalStylishPoint;
		m_draw = &WaveResultUI::StylishPointDraw;
		return;
	}
}

void WaveResultUI::CountTotalStylishPoint()
{
	m_lerpTime += kLerpSpeed;

	if (m_lerpTime.IsMax() && m_frame > kShowWaitTime)
	{
		m_lerpTime.SetMin();
		m_sequence = &WaveResultUI::Disappear;
		m_lerpStartPos = m_pos;
		m_frame = 0;
		return;
	}

	++m_frame;
}

void WaveResultUI::Disappear()
{
	m_lerpTime += kLerpSpeed;
	float temp = Geometry::Easing(m_lerpTime, Geometry::EasingKind::kOutQuart);
	// 戻る
	m_pos = m_lerpStartPos + m_lerpStartPos.Lerp(kInitPos, Geometry::Easing(m_lerpTime, Geometry::EasingKind::kOutQuart));

	if (m_lerpTime.IsMax())
	{
		m_lerpTime.SetMin();
		m_sequence = &WaveResultUI::Disable;
		m_draw = &WaveResultUI::NoDraw;
		return;
	}
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
	ResultTextDraw();
	// 数字出す
	DrawFormatStringToHandle(m_pos.x + kDamageDrawOffset.x, m_pos.y + kDamageDrawOffset.y, 0xffffff, m_fontH,             "ダメージ　　　　　　　：%.0f", std::lerp(0.0f, m_resultData.lock()->m_damageAmount, Geometry::Easing(m_lerpTime, Geometry::EasingKind::kOutQuart)));
}

void WaveResultUI::TimeDraw() const
{
	BoxDraw();
	ResultTextDraw();
	DrawFormatStringToHandle(m_pos.x + kDamageDrawOffset.x, m_pos.y + kDamageDrawOffset.y, 0xffffff, m_fontH,             "ダメージ　　　　　　　：%.0f", m_resultData.lock()->m_damageAmount);
	DrawFormatStringToHandle(m_pos.x + kTimeDrawOffset.x, m_pos.y + kTimeDrawOffset.y, 0xffffff, m_fontH,                 "クリアタイム　　　　　：%.2fs", std::lerp(0.0f, m_resultData.lock()->m_clearTime, Geometry::Easing(m_lerpTime, Geometry::EasingKind::kOutQuart)));
}

void WaveResultUI::StylishPointDraw() const
{
	BoxDraw();
	ResultTextDraw();
	DrawFormatStringToHandle(m_pos.x + kDamageDrawOffset.x, m_pos.y + kDamageDrawOffset.y, 0xffffff, m_fontH,             "ダメージ　　　　　　　：%.0f", m_resultData.lock()->m_damageAmount);
	DrawFormatStringToHandle(m_pos.x + kTimeDrawOffset.x, m_pos.y + kTimeDrawOffset.y, 0xffffff, m_fontH,                 "クリアタイム　　　　　：%.2fs", m_resultData.lock()->m_clearTime);
	DrawFormatStringToHandle(m_pos.x + kStylishPointDrawOffset.x, m_pos.y + kStylishPointDrawOffset.y, 0xffffff, m_fontH, "スタイリッシュポイント：%.0f", std::lerp(0.0f, m_resultData.lock()->m_stylishRankPoint, Geometry::Easing(m_lerpTime, Geometry::EasingKind::kOutQuart)));
}

void WaveResultUI::NoDraw() const
{
}

void WaveResultUI::ResultTextDraw() const
{
	DrawStringToHandle(m_pos.x + kResultTextOffset.x, m_pos.y + kResultTextOffset.y, "Result", 0xffffff, m_fontH);
}