#include "WaveResult.h"
#include <string>
#include "Image.h"
#include <DxLib.h>
#include "../General/Game.h"

namespace
{
	const std::string kFontName = "";
	constexpr int kFontSize = 10;
	constexpr int kFontThick = 1;

	constexpr int kAppearFrame = 60;
	constexpr int kLifeFrame = 180;
	constexpr int kDisappearFrame = 60;

	const Vector3 kInitPos = { Game::kScreenWidth + 500.0f, 500.0f, 0.0f };
	const Vector3 kAppearTargetPos = { Game::kScreenWidth - 500.0f, 500.0f, 0.0f };
	constexpr float kLerpSpeed = 0.3f;
}

WaveResult::WaveResult() :
	m_fontH(-1)
{
}

WaveResult::~WaveResult()
{
	DeleteFontToHandle(m_fontH);
}

void WaveResult::Init()
{
	// �t�H���g���
	m_fontH = CreateFontToHandle(kFontName.c_str(), kFontSize, kFontThick);
}

void WaveResult::Update()
{
	(this->*m_sequence)();
}

void WaveResult::Draw() const
{
	m_image->Draw({ m_pos.x, m_pos.y});
}

void WaveResult::StartResult()
{
	m_pos = kInitPos;
}

void WaveResult::Appear()
{
	// �ɂ��
	m_pos.LerpMyself(kAppearTargetPos, kLerpSpeed);
}

void WaveResult::Wait()
{
	// �΁[��

}

void WaveResult::Disappear()
{
	// �����
	m_pos.LerpMyself(kInitPos, kLerpSpeed);
}