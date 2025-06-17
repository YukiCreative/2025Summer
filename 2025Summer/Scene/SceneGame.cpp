#include "SceneGame.h"
#include <DxLib.h>
#include "SceneController.h"
#include "SceneDebug.h"
#include "Input.h"

SceneGame::SceneGame()
{
}

void SceneGame::Init()
{

}

void SceneGame::Update()
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("GoDebug"))
	{
		SceneController::GetInstance().ChangeScene(std::make_shared<SceneDebug>());
	}
}

void SceneGame::Draw() const 
{
	DrawString(0, 0, "ÉQÅ[ÉÄÉVÅ[ÉìÇ≈Ç∑", 0xffffff);
}

void SceneGame::Entry()
{

}

void SceneGame::Reave()
{

}