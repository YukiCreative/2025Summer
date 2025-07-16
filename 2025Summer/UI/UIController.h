#pragma once
#include <memory>
#include <list>

class UIBase;

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

private:

	UIList_t m_UIList;

};

