#include "SceneController.h"
#include "SceneDebug.h"
#include "SceneTitle.h"
#include <DxLib.h>
#include "SceneTest.h"
#include "../Shader/ShadowMap.h"
#include "../General/Game.h"
#include "../Camera/Camera.h"
#include "../Actor/ActorController.h"
#include "../UI/Image.h"
#include "../General/Input.h"


namespace
{
	const Vector3 kShadowAreaMin = { -1500, -300, -1500 };
	const Vector3 kShadowAreaMax = { 1500,  300,  1500 };

	const std::string kLogoName = "Title.png";
	constexpr float kLogoExRate = 0.5f;
	const Vector2 kLogoPos = { Game::kScreenHalfWidth, Game::kScreenHalfHeight - 50 };

	const Vector2 kStartButtonPos = { Game::kScreenHalfWidth, Game::kScreenHalfHeight + 200 };
	const std::string kStartButtonImage = "PressEny.png";
	constexpr float kBlendParamMax = 255.0f;
	constexpr float kBlinkSpeed = 0.05f;
	constexpr float kRapidBlinkSpeed = 1.0f;
	constexpr float kImageBlendParamLow = 50.0f;
}

SceneTitle::SceneTitle() :
	m_isAnim(false),
	m_frame(0)
{
}

void SceneTitle::Init()
{
	m_camera = std::make_shared<Camera>();
	m_camera->Init();

	m_actors = std::make_shared<ActorController>();
	m_actors->Init();

	m_shadow = std::make_shared<ShadowMap>();
	m_shadow->Init(kShadowAreaMin, kShadowAreaMax);

	m_title = std::make_shared<Image>();
	m_title->Init(kLogoName);
	m_title->SetExRate(kLogoExRate);

	m_pressEnyImage = std::make_shared<Image>();
	m_pressEnyImage->Init(kStartButtonImage);
	m_pressEnyImage->SetImageBlendMode(DX_BLENDMODE_ALPHA, 255.0f);
}

void SceneTitle::Update()
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
		return;
	}

	if (input.IsPressAny())
	{
		// 押したアニメーションを流す
		m_isAnim = true;

		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneTest>());
		return;
	}

	m_camera->Update();
	m_actors->Update();

	UpdatePressEnyImage();
}

void SceneTitle::UpdatePressEnyImage()
{
	// 透明度を変える
	// 出たり消えたりする
	float param;
	if (m_isAnim)
	{
		param = ((sinf(m_frame * kRapidBlinkSpeed) + 1) * 0.5f) * kBlendParamMax;
	}
	else
	{
		param = ((sinf(m_frame * kBlinkSpeed) + 1) * 0.5f) * kBlendParamMax;
	}

	m_pressEnyImage->SetImageBlendParam(std::max(param, kImageBlendParamLow));

	++m_frame;
}

void SceneTitle::Draw() const
{
	DrawFormatString(300, 0, 0xffffff, "FPS:%.2f", GetFPS());

	m_shadow->StartShadowMapDraw(GetLightDirection());

	m_actors->Draw();

	m_shadow->EndShadowMapDraw();

	m_actors->Draw();

	m_shadow->UnsetShadowMap();

	// 3Dの描画が終わった後にUIを描画
	m_title->Draw(kLogoPos);
	m_pressEnyImage->Draw(kStartButtonPos);

	DrawString(0, 0, "タイトルシーンです", 0xffffff);
}

void SceneTitle::Entry()
{

}

void SceneTitle::Reave()
{

}