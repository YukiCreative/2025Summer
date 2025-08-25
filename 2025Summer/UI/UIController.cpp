#include "UIController.h"
#include "UIBase.h"
#include "../General/Input.h"
#include <fstream>
#include <DxLib.h>
#include <string>
#include "UIPositionLoader.h"
#include "../General/Color.h"

namespace
{
	const std::string kOutputFilePath = "Data/UIPositionData/UIPositionData/csv";
	constexpr float kBaseMoveSpeed = 3.0f;
	constexpr float kAdditionalMoveSpeed = 1.0f;
	constexpr float kScaleEditSpeed = 0.1f;
}

UIController::UIController() :
	m_update(&UIController::NormalUpdate),
	m_draw(&UIController::NormalDraw),
	m_moveSpeed(kBaseMoveSpeed)
{
}

void UIController::Init()
{
}

void UIController::Update()
{
	(this->*m_update)();
}

void UIController::Draw() const
{
	(this->*m_draw)();
}

void UIController::AddUI(std::shared_ptr<UIBase> addUI)
{
	if (std::find(m_UIList.begin(), m_UIList.end(), addUI) != m_UIList.end()) return;

	m_UIList.emplace_back(addUI);
}

bool UIController::IsEditing() const
{
	return m_update == &UIController::EditUpdate;
}

void UIController::NormalUpdate()
{
	for (auto& UI : m_UIList)
	{
		UI->Update();
	}

#if _DEBUG
	if (Input::GetInstance().IsTrigger("GoEdit"))
	{
		m_update = &UIController::EditUpdate;
		m_draw = &UIController::EditDraw;
		m_selectedUI = m_UIList.begin();
		return;
	}
#endif
}

void UIController::EditUpdate()
{
	// 編集モード
	auto& input = Input::GetInstance();

	if (m_UIList.size() <= 0) return;

	if (CheckHitKey(KEY_INPUT_O))
	{
		m_moveSpeed += kAdditionalMoveSpeed;
	}
	if (CheckHitKey(KEY_INPUT_L))
	{
		m_moveSpeed -= kAdditionalMoveSpeed;
	}

	if (input.IsTrigger("SelectEditUI"))
	{
		m_selectedUI++;
		if (m_selectedUI == m_UIList.end())
		{
			m_selectedUI = m_UIList.begin();
		}
	}

	const Vector2& nowPos = (*m_selectedUI)->GetPos();
	Vector2 nextPos;

	// 十字キーで移動
	if (input.IsPressed("Up"))
	{
		nextPos = { 0, -m_moveSpeed };
	}
	if (input.IsPressed("Down"))
	{
		nextPos += {0, m_moveSpeed};
	}
	if (input.IsPressed("Left"))
	{
		nextPos += {-m_moveSpeed, 0};
	}
	if (input.IsPressed("Right"))
	{
		nextPos += {m_moveSpeed, 0};
	}

	(*m_selectedUI)->SetPos(nowPos + nextPos);

	// 拡縮
	if (CheckHitKey(KEY_INPUT_RSHIFT))
	{
		(*m_selectedUI)->SetScale((*m_selectedUI)->GetScale() + kScaleEditSpeed);
	}
	if (CheckHitKey(KEY_INPUT_RCONTROL))
	{
		(*m_selectedUI)->SetScale((*m_selectedUI)->GetScale() - kScaleEditSpeed);
	}

	// 保存キーを押したら今出ているUIの座標を保存
	if (input.IsTrigger("EditSave"))
	{
		OutputPosition();
	}

	if (input.IsTrigger("GoEdit"))
	{
		m_update = &UIController::NormalUpdate;
		m_draw = &UIController::NormalDraw;
		return;
	}
}

void UIController::NormalDraw() const
{
	for (auto& UI : m_UIList)
	{
		UI->Draw();
	}
}

void UIController::EditDraw() const
{
	for (auto& UI : m_UIList)
	{
		UI->Draw();
	}

	DrawCircle(static_cast<int>((*m_selectedUI)->GetPos().x), static_cast<int>((*m_selectedUI)->GetPos().y), 10, Color::Red);

	DrawString(500, 30, "Editモードです", 0xffffff);
	//DrawString(700, 30, "※テキストのサイズは変更しにくいため、再起動しないと反映されません", 0xffffff);
	DrawFormatString(500, 45, 0xffffff, "選択中のUI：%s", (*m_selectedUI)->GetName().c_str());
	DrawFormatString(500, 60, 0xffffff, "カーソルのスピード:%.2f", m_moveSpeed);
}

void UIController::OutputPosition()
{
	for (auto& UI : m_UIList)
	{
		// Mapを更新
		UIPositionLoader::GeInstance().SetPosData(UI->GetName(), UI->GetPos(), UI->GetScale());
	}

	UIPositionLoader::GeInstance().SavePosition();
}