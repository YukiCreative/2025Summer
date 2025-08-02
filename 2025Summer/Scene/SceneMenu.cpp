#include "SceneMenu.h"
#include <DxLib.h>
#include "../General/Input.h"
#include "SceneController.h"
#include "SceneDebug.h"

SceneMenu::SceneMenu()
{
}

void SceneMenu::Init()
{

}

void SceneMenu::Update()
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
	}
}

void SceneMenu::Draw() const
{
	DrawString(0, 0, "���j���[�V�[���ł�", 0xffffff);
	DrawString(0, 15, "�Q�[����I��", 0xffffff);
}

void SceneMenu::Entry()
{

}

void SceneMenu::Reave()
{

}