#pragma once
#include "Scene.h"
#include <string>
#include <list>

class SceneDebug : public Scene
{
public:
	SceneDebug();

	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	virtual void Entry();
	virtual void Reave();

private:

	std::list<std::string> m_menuList;
	int m_selectNum;

};

