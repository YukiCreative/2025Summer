#include "SceneMenu.h"
#include <DxLib.h>
#include "../General/Input.h"
#include "SceneController.h"
#include "SceneDebug.h"
#include "../UI/Image.h"
#include <cassert>
#include "../UI/ButtonSystem.h"
#include "../UI/UIController.h"

#include "SceneTutorial.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneTest.h"
#include "SceneOption.h"
#include "SceneRanking.h"

namespace
{
	constexpr int kMenuNum = 6;
	const Vector2 kButtonPos = {100, 100};
	const Vector2 kButtonPosOffset = {0, 50};
}

SceneMenu::SceneMenu()
{
}

void SceneMenu::Init()
{
	auto buttons = std::make_shared<ButtonSystem>();
	buttons->Init();

	for (int i = 0; i < kMenuNum; ++i)
	{
		auto button = std::make_shared<MenuButton>();
		button->Init(kButtonPos + kButtonPosOffset * static_cast<float>(i), static_cast<TitleTransitionSceneKind>(i));
		buttons->AddButton(button);
		buttons->SetButtonFocus(button);
	}

	m_UI = std::make_shared<UIController>();
	m_UI->Init();

	m_UI->AddUI(buttons);
}

void SceneMenu::Update()
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
	}

	m_UI->Update();
}

void SceneMenu::Draw() const
{
	DrawString(0, 0, "メニューシーンです", 0xffffff);
	DrawString(0, 15, "ゲームを選択", 0xffffff);

	m_UI->Draw();
}

void SceneMenu::Entry()
{

}

void SceneMenu::Reave()
{

}

// ===============================================

SceneMenu::MenuButton::MenuButton() :
	Button(),
	m_kind(TitleTransitionSceneKind::kNone)
{
}

void SceneMenu::MenuButton::Init(const Vector2 initPos, const TitleTransitionSceneKind kind)
{
	m_kind = kind;

	Button::Init(initPos);

	m_image = std::make_shared<Image>();
	m_image->Init("SpecialGaugeBase.png");
}

void SceneMenu::MenuButton::Draw() const
{
	m_image->Draw(m_pos);
}

void SceneMenu::MenuButton::OnFocused()
{
	// 仮
	m_image->SetExRatio(2.0f);
}

void SceneMenu::MenuButton::OnDisfocused()
{
	m_image->SetExRatio(1.0f);
}

void SceneMenu::MenuButton::OnPressed()
{
	// 自分の設定されたシーンへ移行
	switch (m_kind)
	{
	case SceneMenu::TitleTransitionSceneKind::kTutorial:
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneTutorial>());
		break;
	case SceneMenu::TitleTransitionSceneKind::kCampaign:
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneGame>());
		break;
	case SceneMenu::TitleTransitionSceneKind::kArena:
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneTest>());
		break;
	case SceneMenu::TitleTransitionSceneKind::kRanking:
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneRanking>());
		break;
	case SceneMenu::TitleTransitionSceneKind::kOption:
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneOption>());
		break;
	case SceneMenu::TitleTransitionSceneKind::kTitle:
		SceneController::GetInstance().ChangeSceneWithFade(std::make_shared<SceneTitle>());
		break;
	default:
		assert(false && "遷移先が設定されてないボタンがあるよ");
		break;
	}
}

void SceneMenu::MenuButton::FocusedUpdate()
{
}

void SceneMenu::MenuButton::NormalUpdate()
{
}
