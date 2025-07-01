#include "ButtonSystem.h"
#include "Input.h"
#include "SceneController.h"
#include "SceneDebug.h"
#include "SceneTitle.h"
#include <DxLib.h>

SceneTitle::SceneTitle()
{
}

void SceneTitle::Init()
{
	m_buttons = std::make_shared<ButtonSystem>();
}

void SceneTitle::Update()
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
		return;
	}

	m_buttons->Update();
}

void SceneTitle::Draw() const
{
	DrawString(0, 0, "タイトルシーンです", 0xffffff);
}

void SceneTitle::Entry()
{

}

void SceneTitle::Reave()
{

}