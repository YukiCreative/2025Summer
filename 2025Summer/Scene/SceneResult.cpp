#include "SceneResult.h"
#include <DxLib.h>
#include "SceneController.h"
#include "SceneDebug.h"
#include "../General/Input.h"
#include "../UI/UIController.h"
#include "../GameManagement/Score/ScoreManager.h"
#include "../UI/Image.h"
#include "SceneTitle.h"
#include "../Model/AnimationModel.h"
#include "../General/Game.h"
namespace
{
	const std::string kResultTextImgName = "Result.png";
	const std::string kWavesTextImgName = "Waves.png";
	const std::string kTotalTextImgName = "Total.png";

	const std::string kModelName = "Data/Model/Player.mv1";

	const std::string kSRankImgName = "SRankFill.png";
	const std::string kARankImgName = "ARankFill.png";
	const std::string kBRankImgName = "BRankFill.png";
	const std::string kCRankImgName = "CRankFill.png";
	const std::string kDRankImgName = "DRankFill.png";

	const std::string kSRankPose = "Armature|SRankPose";
	const std::string kARankPose = "Armature|ARankPose";
	const std::string kBRankPose = "Armature|BRankPose";
	const std::string kCRankPose = "Armature|CRankPose";
	const std::string kDRankPose = "Armature|DRankPose";

	const Vector2 kResultImgInitPos = { 300.0f, 100.0f };
	const Vector2 kWavesImgInitPos = { 200.0f, 300.0f };
	const Vector2 kTotalImgInitPos = { 200.0f, 500.0f };

	constexpr float kRankImageTargetExRatio = 0.1f;
	constexpr float kRankImageStartExRatio = 1.0f;
	const Vector2 kRankStartPos = { 400.0f, 300.0f };
	const Vector2 kRankImageOffset = { 50.0f, 0.0f };

	constexpr int kOneWaveRankImgAppearFrame = 20;
	constexpr float kShowTotalRankFrame = 60.0f;

	constexpr float kTotalRankStartExRatio = 1.5f;
	constexpr float kTotalRankTargetExRatio = 0.7f;
	const Vector2 kTotalRankDrawPos = { 450.0f, 500.0f };

	constexpr float kShowTrophyFrame = 120.0f;
	constexpr float kRotateTrophySpeed = 0.02f;
	const Vector3 kTrophyDrawPos = {100.0f, -100.0f, 0.0f};

	const Vector3 kCameraPos = {0.0f, 0.0f, -200.0f};
	const Vector3 kCameraTarget = {0.0f, 0.0f, 0.0f};

	const int kBackGroundColor[3] = {230, 230, 230};

	const std::string kFontName = "BIZ UDP明朝 Medium";
	constexpr int kFontSize = 15;
	constexpr int kFontThick = 1;
}

SceneResult::SceneResult() :
	m_frame(0),
	m_sequence(&SceneResult::ShowWaveRank),
	m_backGroundH(-1)
{
}

void SceneResult::Init()
{
	m_UI = std::make_shared<UIController>();
	m_UI->Init();

	m_backGroundH = MakeGraph(Game::kScreenWidth, Game::kScreenHeight);
	FillGraph(m_backGroundH, kBackGroundColor[0], kBackGroundColor[1], kBackGroundColor[2]);

	m_fontH = CreateFontToHandle(kFontName.c_str(), kFontSize, kFontThick);

	m_resultTextImage = std::make_shared<Image>();
	m_resultTextImage->Init(kResultTextImgName);
	m_wavesTextImage = std::make_shared<Image>();
	m_wavesTextImage->Init(kWavesTextImgName);
	m_totalTextImage = std::make_shared<Image>();
	m_totalTextImage->Init(kTotalTextImgName);
	
	// 全ウェーブのランクを取得
	m_waveRanks = ScoreManager::GetInstance().CalcAllWaveRank();

	m_waveRankImages.resize(m_waveRanks.size());

	for (int i = 0; auto& img : m_waveRankImages)
	{
		img = std::make_shared<Image>();

		switch (m_waveRanks[i])
		{
		case WaveRank::kSRank:
			img->Init(kSRankImgName);
			break;
		case WaveRank::kARank:
			img->Init(kARankImgName);
			break;
		case WaveRank::kBRank:
			img->Init(kBRankImgName);
			break;
		case WaveRank::kCRank:
			img->Init(kCRankImgName);
			break;
		case WaveRank::kDRank:
			img->Init(kDRankImgName);
			break;
		default:
			break;
		}

		img->SetExRatio(kRankImageStartExRatio);
		img->SetImageBlendMode(DX_BLENDMODE_ALPHA, 0);

		++i;
	}

	m_totalRankImg = std::make_shared<Image>();

	m_trophyModel = std::make_shared<AnimationModel>();
	m_trophyModel->Init(kModelName, 0.0f);
	m_trophyModel->SetScale(Vector3::Zero());
	m_trophyModel->SetPos(kTrophyDrawPos);

	// 画像、モデルの設定
	switch (ScoreManager::GetInstance().CalcAllWaveOverallResult())
	{
		// 二回アニメーション切り替えてるのはブレンドを無効化するため
	case WaveRank::kSRank:
		m_totalRankImg->Init(kSRankImgName);
		m_trophyModel->ChangeAnimation(kSRankPose);
		m_trophyModel->ChangeAnimation(kSRankPose);
		break;
	case WaveRank::kARank:
		m_totalRankImg->Init(kARankImgName);
		m_trophyModel->ChangeAnimation(kARankPose);
		m_trophyModel->ChangeAnimation(kARankPose);
		break;
	case WaveRank::kBRank:
		m_totalRankImg->Init(kBRankImgName);
		m_trophyModel->ChangeAnimation(kBRankPose);
		m_trophyModel->ChangeAnimation(kBRankPose);
		break;
	case WaveRank::kCRank:
		m_totalRankImg->Init(kCRankImgName);
		m_trophyModel->ChangeAnimation(kCRankPose);
		m_trophyModel->ChangeAnimation(kCRankPose);
		break;
	case WaveRank::kDRank:
		m_totalRankImg->Init(kDRankImgName);
		m_trophyModel->ChangeAnimation(kDRankPose);
		m_trophyModel->ChangeAnimation(kDRankPose);

		break;
	default:
		break;
	}

	m_totalRankImg->SetImageBlendMode(DX_BLENDMODE_ALPHA, 0);

	SetCameraPositionAndTarget_UpVecY(kCameraPos, kCameraTarget);

	SetLightDirection(kCameraTarget - kCameraPos);
}

void SceneResult::Update()
{
	Input& input = Input::GetInstance();

#if _DEBUG

	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
		return;
	}

#endif

	(this->*m_sequence)();
}

void SceneResult::Draw() const
{
	DrawGraph(0, 0, m_backGroundH, true);

	DrawString(0, 0, "リザルトシーンです", 0xffffff);

	m_resultTextImage->Draw(kResultImgInitPos);
	m_wavesTextImage->Draw(kWavesImgInitPos);
	m_totalTextImage->Draw(kTotalImgInitPos);

	for (int i = 0; auto& img : m_waveRankImages)
	{
		img->Draw(kRankStartPos + kRankImageOffset * i);

		++i;
	}

	m_totalRankImg->Draw(kTotalRankDrawPos);

	m_trophyModel->Draw();

	DrawStringToHandle(600, 300, "何かボタンを押して戻る", 0x000000, m_fontH, 0xffffff);
}

void SceneResult::Entry()
{

}

void SceneResult::Reave()
{

}

void SceneResult::ShowWaveRank()
{
	// ウェーブごとのランクを表示していく

	for (int i = 0; auto& img : m_waveRankImages)
	{
		float ratio = static_cast<float>(std::clamp(m_frame - kOneWaveRankImgAppearFrame * i, 0, kOneWaveRankImgAppearFrame))
			/ static_cast<float>(kOneWaveRankImgAppearFrame);

		img->SetImageBlendParam(ratio * 255.0f);
		img->SetExRatio(std::lerp(kRankImageStartExRatio, kRankImageTargetExRatio, ratio));
		++i;
	}

	++m_frame;

	// 全部表示したらShowTotalRankへ
	if (m_waveRankImages.back()->GetBlendParam() > 255.0f - Geometry::kEpsilon)
	{
		m_sequence = &SceneResult::ShowTotalRank;
		m_frame = 0;
		return;
	}
}

void SceneResult::ShowTotalRank()
{
	// 総合ランク出す
	float ratio = Geometry::Easing(static_cast<float>(m_frame) / kShowTotalRankFrame, Geometry::EasingKind::kInExpo);

	m_totalRankImg->SetExRatio(std::lerp(kTotalRankStartExRatio, kTotalRankTargetExRatio, ratio));
	m_totalRankImg->SetImageBlendParam(ratio * 255.0f);

	++m_frame;

	// 出したらShowRankTrophyへ
	if (ratio >= 1.0f - Geometry::kEpsilon)
	{
		m_sequence = &SceneResult::ShowRankTrophy;
		m_frame = 0;
		return;
	}
}

void SceneResult::ShowRankTrophy()
{
	float ratio = Geometry::Easing(static_cast<float>(m_frame) / kShowTrophyFrame, Geometry::EasingKind::kOutElastic);

	// モデルを出す
	m_trophyModel->SetScale({ratio, ratio, ratio});
	m_trophyModel->RotateUpVecY(kRotateTrophySpeed);

	++m_frame;

	// 出したらWait
	if (m_frame >= kShowTrophyFrame)
	{
		m_frame = 0;
		m_sequence = &SceneResult::Wait;
		return;
	}
}

void SceneResult::Wait()
{
	// モデル回す
	m_trophyModel->RotateUpVecY(kRotateTrophySpeed);

	// 入力があればシーン遷移
	if (Input::GetInstance().IsTrigger("Submit"))
	{
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneTitle>());
		return;
	}
}
