#include "ScreenFade.h"
#include <DxLib.h>
#include "Game.h"
#include "Image.h"

namespace
{
	// 255 / �t���[����
	constexpr float kFadeSpeed = 255.0f / 40.0f;
}

ScreenFade::ScreenFade() :
	m_fadeImage(),
	m_isFadeIn(true),
	m_fadeParam()
{
}

ScreenFade::~ScreenFade()
{
	m_fadeImage->DeleteImage();
}

void ScreenFade::Init()
{
	// ���̉摜�̃T�C�Y�Ő^�����ȉ摜�����
	auto fadeImage = MakeGraph(Game::kScreenWidth, Game::kScreenHeight);
	FillGraph(fadeImage, 0, 0, 0);

	m_fadeImage = std::make_shared<Image>();
	m_fadeImage->Init(fadeImage);
	m_fadeParam.SetMax();
	m_fadeImage->SetImageBlendMode(DX_BLENDMODE_ALPHA, m_fadeParam.Value());

}

void ScreenFade::Update()
{
	if (m_isFadeIn)
	{
		// �����x��������
		// �Ƃ肠�����Œ�l
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

bool ScreenFade::IsEndFadeOut() const
{
	return m_fadeParam.IsMax();
}

bool ScreenFade::IsEndFadeIn() const
{
	return m_fadeParam.IsMin();
}

void ScreenFade::FadeIn()
{
	m_isFadeIn = true;
}

void ScreenFade::FadeOut()
{
	m_isFadeIn = false;
}
