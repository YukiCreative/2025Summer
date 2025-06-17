#pragma once
#include "Scene.h"

class SceneTitle : public Scene
{
public:

	SceneTitle();

	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	virtual void Entry();
	virtual void Reave();
};

