#pragma once
#include "Scene.h"

class SceneRanking : public Scene
{
public:

	SceneRanking();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void Entry() override;
	void Reave() override;


private:
};

