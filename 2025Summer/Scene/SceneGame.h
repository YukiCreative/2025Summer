#pragma once
#include "Scene.h"
class SceneGame : public Scene
{
public:

	SceneGame();

	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	virtual void Entry();
	virtual void Reave();
};

