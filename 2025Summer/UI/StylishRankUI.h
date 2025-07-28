#pragma once
#include "UIBase.h"
#include "StylishRank.h"
#include <unordered_map>
#include <string>

class Image;

class StylishRankUI : public UIBase
{
public:
	void Init();

	void Update() override;

	void Draw() const override;

private:

	StylishRankUIParam m_nowParam;
	std::shared_ptr<Image> m_baseImage;
	std::shared_ptr<Image> m_fillImage;
	std::unordered_map<StylishRankKind, std::string> m_rankBaseImageMap;
	std::unordered_map<StylishRankKind, std::string> m_rankFillImageMap;

	RangeLimitedValue<float, 1.0f, 2.0f> m_exRate;
	RangeLimitedValue<float, 0.0f, 255.0f> m_alphaRate;

private:

	void InitMap();
	void ResetAnim();
};

