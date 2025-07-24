#pragma once
#include "Scene.h"
#include <string>
#include <memory>
#include <list>

class Image;

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

	std::shared_ptr<Image> a;
};

