#pragma once
#include "Scene.h"
class SceneMenu : public Scene
{
	SceneMenu();

	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	virtual void Entry();
	virtual void Reave();
};

