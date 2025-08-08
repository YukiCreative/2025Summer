#pragma once
#include "Scene.h"

class SceneOption : public Scene
{
public:

	SceneOption();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void Entry() override;
	void Reave() override;

private:

};

