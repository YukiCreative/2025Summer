#pragma once
#include "Scene.h"
#include "Vector3.h"
#include <memory>

class Camera;

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
	std::shared_ptr<Camera> m_camera;
};