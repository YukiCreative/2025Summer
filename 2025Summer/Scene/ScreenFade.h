#pragma once
#include "Image.h"
#include <memory>
#include "RangeLimitedValue.h"

// �t�F�[�h�C���A�E�g�������ǂ�܂�
class ScreenFade
{
public:

	ScreenFade();
	~ScreenFade();

	void Init();

	void Update();

	void Draw() const;

	bool IsEndFadeOut() const;
	bool IsEndFadeIn() const;

	void FadeIn();
	void FadeOut();
private:

	// true�Ȃ�t�F�[�h�C���Afalse�Ȃ�t�F�[�h�A�E�g
	bool m_isFadeIn;

	std::shared_ptr<Image> m_fadeImage;

	using FadeParam = RangeLimitedValue<float, 0.0f, 255.0f>;
	FadeParam m_fadeParam;
};