#include "UIController.h"
#include "UIBase.h"

UIController::UIController()
{
}

void UIController::Init()
{

}

void UIController::Update()
{
	for (auto& UI : m_UIList)
	{
		UI->Update();
	}
}

void UIController::Draw() const
{
	for (auto& UI : m_UIList)
	{
		UI->Draw();
	}
}

void UIController::AddUI(std::shared_ptr<UIBase> addUI)
{
	if (std::find(m_UIList.begin(), m_UIList.end(), addUI) != m_UIList.end()) return;

	m_UIList.emplace_back(addUI);
}
