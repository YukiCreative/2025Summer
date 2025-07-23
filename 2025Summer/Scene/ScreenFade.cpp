#include "ScreenFade.h"
#include <DxLib.h>
#include "Game.h"
#include "Image.h"

namespace
{
	constexpr float kFadeSpeed = 0.02f;
}

ScreenFade::ScreenFade() :
	m_fadeImage()
{
}

ScreenFade::~ScreenFade()
{
	m_fadeImage->DeleteImage();
	m_fadeParam.SetMin();
}

void ScreenFade::Init()
{
	// 今の画像のサイズで真っ黒な画像を作る
	auto fadeImage = MakeGraph(Game::kScreenWidth, Game::kScreenHeight);
	FillGraph(fadeImage, 0, 0, 0);

	m_fadeImage = std::make_shared<Image>();
	m_fadeImage->Init(fadeImage);
	m_fadeImage->SetImageBlendMode(DX_BLENDMODE_ALPHA, 0);
}

void ScreenFade::Update()
{
	if (m_isFadeIn)
	{
		// 透明度を下げる
		m_fadeParam -= kFadeSpeed;
	}
	else
	{
		m_fadeParam += kFadeSpeed;
	}
	
	m_fadeImage->SetImageBlendParam(m_fadeParam.Value());
}

void ScreenFade::Draw() const
{
	m_fadeImage->Draw({ Game::kScreenHalfWidth, Game::kScreenHalfHeight });
}