#include "ButtonSystem.h"
#include "Input.h"
#include "SceneController.h"
#include "SceneDebug.h"
#include "SceneTitle.h"
#include <DxLib.h>
#include "UIController.h"
#include "ActorController.h"
#include "Camera.h"
#include "ShadowMap.h"
#include "Game.h"
#include "Image.h"
#include "SceneTest.h"

namespace
{
	const Vector3 kShadowAreaMin = { -1500, -300, -1500 };
	const Vector3 kShadowAreaMax = { 1500,  300,  1500 };

	const Vector2 kStartButtionPos = { Game::kScreenHalfWidth, Game::kScreenHalfHeight + 200 };

	const std::string kLogoName = "Title.png";
	constexpr float kLogoExRate = 0.5f;
	const Vector2 kLogoPos = { Game::kScreenHalfWidth, Game::kScreenHalfHeight - 50 };
}

SceneTitle::SceneTitle()
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

	m_UI = std::make_shared<UIController>();
	m_UI->Init();

	auto buttons = std::make_shared<ButtonSystem>();
	buttons->Init();

	auto startButton = std::make_shared<StartButton>();
	startButton->Init(kStartButtionPos);

	buttons->AddButton(startButton);
	buttons->SetButtonFocus(startButton);

	m_UI->AddUI(buttons);

	m_title = std::make_shared<Image>();
	m_title->Init(kLogoName);
	m_title->SetExRate(kLogoExRate);
}

void SceneTitle::Update()
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
		return;
	}

	m_camera->Update();
	m_actors->Update();
	m_UI->Update();
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
	m_UI->Draw();
	m_title->Draw(kLogoPos);

	DrawString(0, 0, "タイトルシーンです", 0xffffff);
}

void SceneTitle::Entry()
{

}

void SceneTitle::Reave()
{

}

// ===============================================

namespace
{
	const std::string kStartButtonImage = "PressEny.png";
	constexpr float kBlendParamMax = 255.0f;
	constexpr float kBlinkSpeed = 0.05f;
	constexpr float kRapidBlinkSpeed = 1.0f;
	constexpr float kImageBlendParamLow = 50.0f;
}

SceneTitle::StartButton::StartButton() :
	m_isAnim(false),
	m_frame(0),
	Button()
{
}

void SceneTitle::StartButton::Init(const Vector2& initPos)
{
	Button::Init(initPos);

	m_image = std::make_shared<Image>();
	m_image->Init(kStartButtonImage);
	m_image->SetImageBlendMode(DX_BLENDMODE_ALPHA, 255.0f);
}

void SceneTitle::StartButton::Draw() const
{
	m_image->Draw(m_pos);
}

void SceneTitle::StartButton::OnFocused()
{
}

void SceneTitle::StartButton::OnDisfocused()
{
}

void SceneTitle::StartButton::OnPressed()
{
	// 押したアニメーションを流す
	m_isAnim = true;

	SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneTest>());
}

void SceneTitle::StartButton::FocusedUpdate()
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

	m_image->SetImageBlendParam(std::max(param, kImageBlendParamLow));

	++m_frame;
}

void SceneTitle::StartButton::NormalUpdate()
{
}