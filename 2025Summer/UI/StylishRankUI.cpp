#include "StylishRankUI.h"
#include "Image.h"

namespace
{
	const std::string kNoImageName  = "StylishRankNoImage.png";
	const std::string kDImageName   = "DRankBase.png";
	const std::string kCImageName   = "CRankBase.png";
	const std::string kBImageName   = "BRankBase.png";
	const std::string kAImageName   = "ARankBase.png";
	const std::string kSImageName   = "SRankBase.png";
	const std::string kSSImageName  = "SSRankBase.png";
	const std::string kSSSImageName = "SSSRankBase.png";

	const std::string kNoImageFill  = "StylishRankNoImage.png";
	const std::string kDImageFill   = "DRankFill.png";
	const std::string kCImageFill   = "CRankFill.png";
	const std::string kBImageFill   = "BRankFill.png";
	const std::string kAImageFill   = "ARankFill.png";
	const std::string kSImageFill   = "SRankFill.png";
	const std::string kSSImageFill  = "SSRankFill.png";
	const std::string kSSSImageFill = "SSSRankFill.png";

	constexpr float kImageShrinkingSpeed = 0.1f;
	constexpr float kImageAppearSpeed = 20.0f;

	const Vector3 kInitPos = {900.0f, 400.0f, 0.0f};
}

void StylishRankUI::Init()
{
	InitMap();

	m_baseImage = std::make_shared<Image>();
	m_baseImage->Init(kNoImageName);
	m_baseImage->SetImageBlendMode(DX_BLENDMODE_ALPHA);
	m_fillImage = std::make_shared<Image>();
	m_fillImage->Init(kNoImageFill);

	m_pos = kInitPos;

	ResetAnim();
}

void StylishRankUI::Update()
{
	// データ取得
	auto param = StylishRank::GetInstance().GetRankUIParam();

	// ランクが今のと変わっていたら画像を変える
	if (param.m_rank != m_nowParam.m_rank)
	{
		m_baseImage->SetGraph(m_rankBaseImageMap[param.m_rank]);
		m_fillImage->SetGraph(m_rankFillImageMap[param.m_rank]);

		// あとアニメーションする
		ResetAnim();
	}

	m_nowParam = param;

	m_baseImage->SetExRate(m_exRate.Value());
	m_fillImage->SetExRate(m_exRate.Value());
	m_baseImage->SetImageBlendParam(m_alphaRate.Value());

	m_exRate -= kImageShrinkingSpeed;
	m_alphaRate += kImageAppearSpeed;
}

void StylishRankUI::Draw() const
{
	const Vector2 drawPos = { m_pos.x, m_pos.y };
	const Vector2 imgSize = m_fillImage->GetImageSize();
	const float ratio = 1 - m_nowParam.m_ratio.Value();
	const float offsetY = imgSize.y * ratio;

	// ランクの割合に併せて画像を表示
	m_baseImage->Draw(drawPos);
	m_fillImage->RectRotaDraw({ drawPos.x , drawPos.y + offsetY * m_exRate.Value() * 0.5f },
		{ 0.0f, offsetY }, { imgSize.x, imgSize.y });
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