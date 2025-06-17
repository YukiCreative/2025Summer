#include "SceneTitle.h"
#include <DxLib.h>
#include "Input.h"
#include "SceneController.h"
#include "SceneDebug.h"

SceneTitle::SceneTitle()
{
}

void SceneTitle::Init()
{

}

void SceneTitle::Update()
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
	}
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