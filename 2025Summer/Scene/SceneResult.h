#pragma once
#include "Scene.h"
class SceneResult : public Scene
{
public:
	SceneResult();

	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	virtual void Entry();
	virtual void Reave();
};

