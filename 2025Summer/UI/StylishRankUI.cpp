#include "StylishRankUI.h"
#include "Image.h"

namespace
{
	const std::string kNoImageName  = "";
	const std::string kDImageName   = "";
	const std::string kCImageName   = "";
	const std::string kBImageName   = "";
	const std::string kAImageName   = "";
	const std::string kSImageName   = "";
	const std::string kSSImageName  = "";
	const std::string kSSSImageName = "";

	const std::string kNoImageFill  = "";
	const std::string kDImageFill   = "";
	const std::string kCImageFill   = "";
	const std::string kBImageFill   = "";
	const std::string kAImageFill   = "";
	const std::string kSImageFill   = "";
	const std::string kSSImageFill  = "";
	const std::string kSSSImageFill = "";

	constexpr float kImageShrinkingSpeed = 0.01f;
	constexpr float kImageAppearSpeed = 5.0f;

	const Vector2 kInitPos = {600.0f, 300.0f};
}

void StylishRankUI::Init()
{
	InitMap();

	m_baseImage = std::make_shared<Image>();
	//m_baseImage->Init(kNoImageName);
	m_fillImage = std::make_shared<Image>();
	//m_fillImage->Init(kNoImageName);

	ResetAnim();
}

void StylishRankUI::Update()
{
	// データ取得
	auto param = StylishRank::GetInstance().GetRankUIParam();

	// ランクが今のと変わっていたら画像を変える
	if (param.m_rank != m_nowParam.m_rank)
	{
		//m_baseImage->SetGraph(m_rankBaseImageMap[param.m_rank]);
		//m_fillImage->SetGraph(m_rankFillImageMap[param.m_rank]);

		// あとアニメーションする
		ResetAnim();
	}

	m_nowParam = param;

	m_baseImage->SetExRate(m_exRate.Value());
	m_baseImage->SetImageBlendParam(m_alphaRate.Value());

	m_exRate -= kImageShrinkingSpeed;
	m_alphaRate += kImageAppearSpeed;
}

void StylishRankUI::Draw() const
{
	const Vector2 drawPos = { m_pos.x, m_pos.y };
	const Vector2 imgSize = m_fillImage->GetImageSize();
	// ランクの割合に併せて画像を表示
	m_baseImage->Draw(drawPos);
	m_fillImage->RectDraw(drawPos + imgSize * 0.5f,
		{0.0f, imgSize.y * (1 - m_nowParam.m_ratio.Value())}, {imgSize.x, imgSize.y});
}

void StylishRankUI::InitMap()
{
	m_rankBaseImageMap[StylishRankKind::kNoRank] = kNoImageName;
	m_rankBaseImageMap[StylishRankKind::kDRank] = kDImageName;
	m_rankBaseImageMap[StylishRankKind::kCRank] = kCImageName;
	m_rankBaseImageMap[StylishRankKind::kBRank] = kBImageName;
	m_rankBaseImageMap[StylishRankKind::kARank] = kAImageName;
	m_rankBaseImageMap[StylishRankKind::kSRank] = kSImageName;
	m_rankBaseImageMap[StylishRankKind::kSSRank] = kSSImageName;
	m_rankBaseImageMap[StylishRankKind::kSSSRank] = kSSSImageName;

	m_rankFillImageMap[StylishRankKind::kNoRank] = kNoImageFill;
	m_rankFillImageMap[StylishRankKind::kDRank] = kDImageFill;
	m_rankFillImageMap[StylishRankKind::kCRank] = kCImageFill;
	m_rankFillImageMap[StylishRankKind::kBRank] = kBImageFill;
	m_rankFillImageMap[StylishRankKind::kARank] = kAImageFill;
	m_rankFillImageMap[StylishRankKind::kSRank] = kSImageFill;
	m_rankFillImageMap[StylishRankKind::kSSRank] = kSSImageFill;
	m_rankFillImageMap[StylishRankKind::kSSSRank] = kSSSImageFill;
}

void StylishRankUI::ResetAnim()
{
	m_exRate.SetMax();
	m_alphaRate.SetMin();
}