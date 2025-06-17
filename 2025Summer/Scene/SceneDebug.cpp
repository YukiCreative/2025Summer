#include "SceneDebug.h"
#include <DxLib.h>
#include "Input.h"
#include "SceneController.h"
#include "SceneTitle.h"
#include "SceneMenu.h"
#include "SceneGame.h"
#include "SceneResult.h"

SceneDebug::SceneDebug() : 
	m_selectNum(0)
{

}

void SceneDebug::Init()
{
	m_menuList.emplace_back("Title");
	m_menuList.emplace_back("Menu");
	m_menuList.emplace_back("Game");
	m_menuList.emplace_back("Result");
}

void SceneDebug::Update()
{
	Input& input = Input::GetInstance();
	if (input.IsTrigger("Up"))
	{
		m_selectNum = (m_selectNum - 1 + m_menuList.size()) % m_menuList.size();
	}
	if (input.IsTrigger("Down"))
	{
		m_selectNum = (m_selectNum + 1 + m_menuList.size()) % m_menuList.size();
	}

	if (input.IsTrigger("Submit"))
	{
		switch (m_selectNum)
		{
		case 0:
			SceneController::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
			break;
		case 1:
			SceneController::GetInstance().ChangeScene(std::make_shared<SceneMenu>());
			break;
		case 2:
			SceneController::GetInstance().ChangeScene(std::make_shared<SceneGame>());
			break;
		case 3:
			SceneController::GetInstance().ChangeScene(std::make_shared<SceneResult>());
			break;
		default:
#if _DEBUG
			printf("シーンがSwitch文に設定されてないよ");
#endif
			break;
		}
	}
}

void SceneDebug::Draw() const
{
	DrawString(0,0,"デバッグメニュー", 0xffffff);

	for (int i = 0;auto& str : m_menuList)
	{
		unsigned int color = 0xffffff;
		if (i == m_selectNum)
		{
			color = 0xfff000;
		}
		DrawString(0, 15 + 15*i, str.c_str(), color);
		++i;
	}

	DrawString(200, 0, "それぞれのシーンでF1キーを押したらデバッグシーンに戻ります", 0xffffff);
}

void SceneDebug::Entry()
{

}

void SceneDebug::Reave()
{

}