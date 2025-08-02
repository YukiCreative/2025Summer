#include "SceneResult.h"
#include <DxLib.h>
#include "SceneController.h"
#include "SceneDebug.h"
#include "../General/Input.h"

SceneResult::SceneResult()
{
}

void SceneResult::Init()
{

}

void SceneResult::Update()
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
	}
}

void SceneResult::Draw() const
{
	DrawString(0, 0, "リザルトシーンです", 0xffffff);
}

void SceneResult::Entry()
{

}

void SceneResult::Reave()
{

}