#pragma once
#include "Scene.h"
#include "../Geometry/Vector3.h"
#include <memory>

class Camera;
class ActorController;
class UIController;
class Model;
class ShadowMap;
class ArenaMode;
class SkyBox;

class SceneTest : public Scene
{
public:
	SceneTest();
	~SceneTest();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void Entry() override;
	void Reave() override;

private:
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<ActorController> m_actors;
	std::shared_ptr<UIController> m_UI;
	std::shared_ptr<ShadowMap> m_shadow;
	std::shared_ptr<ArenaMode> m_gameManager;

	std::shared_ptr<SkyBox> m_skyBox;

private:
};