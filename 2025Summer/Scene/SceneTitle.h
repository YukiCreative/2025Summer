#pragma once
#include "Scene.h"
#include <memory>
#include "Button.h"

class ButtonSystem;
class ActorController;
class UIController;
class ShadowMap;
class Camera;
class Image;

class SceneTitle : public Scene
{
public:

	SceneTitle();

	virtual void Init();
	virtual void Update();

	virtual void Draw() const;

	virtual void Entry();
	virtual void Reave();

private:

	std::shared_ptr<ActorController> m_actors;
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<ShadowMap> m_shadow;
	std::shared_ptr<Image> m_title;

	std::shared_ptr<Image> m_pressEnyImage;
	bool m_isAnim;
	int m_frame;

private:

	void UpdatePressEnyImage();
};