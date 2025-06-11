#pragma once
#include "Scene.h"
#include "Vector3.h"

class SceneTest : public Scene
{
public:
	SceneTest();

	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	virtual void Entry();
	virtual void Reave();

private:

};