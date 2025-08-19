#pragma once
#include <memory>
#include <list>
#include <unordered_map>
#include "UIKind.h"
#include <functional>

class UIBase;
class UIPositionLoader;

using UIList_t = std::list<std::shared_ptr<UIBase>>;

// UI‚ð‚Ü‚Æ‚ß‚Ü‚·
class UIController
{
public:
	UIController();
	
	void Init();

	void Update();
	void Draw() const;

	void AddUI(std::shared_ptr<UIBase>);

	bool IsEditing() const;

private:

	UIList_t m_UIList;
	using UpdateState_t = void (UIController::*)();
	UpdateState_t m_update;
	using DrawState_t = void (UIController::*)()const;
	DrawState_t m_draw;
	UIList_t::iterator m_selectedUI;
	float m_moveSpeed;

private:

	void NormalUpdate();
	void EditUpdate();

	void NormalDraw() const;
	void EditDraw() const;

	void OutputPosition();
};

