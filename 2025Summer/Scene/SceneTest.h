#pragma once
#include "Scene.h"
#include "Vector3.h"
#include <memory>

class Camera;
class ActorController;
class UIController;
class Model;
class ShadowMap;
class EnemyGenerator;

class SceneTest : public Scene
{
public:
	SceneTest();
	~SceneTest();

	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

	virtual void Entry();
	virtual void Reave();

private:
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<ActorController> m_actors;
	std::shared_ptr<UIController> m_UI;
	std::shared_ptr<EnemyGenerator> m_enemyGenerator;
	std::shared_ptr<ShadowMap> m_shadow;

	std::shared_ptr<Model> m_field;

	int m_wave;

private:
	void DrawGrid(const int xNum, const int zNum) const;
};