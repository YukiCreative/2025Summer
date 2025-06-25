#pragma once
#include "Scene.h"
#include <memory>

class ButtonSystem;

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

	std::shared_ptr<ButtonSystem> m_buttons;
};