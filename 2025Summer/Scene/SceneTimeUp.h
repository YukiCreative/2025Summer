#pragma once
#include "Scene.h"
#include <memory>

class Image;

class SceneTimeUp : public Scene
{
public:
	SceneTimeUp();
	~SceneTimeUp();

	void Init() override;

	void Update() override;
	void Draw() const override;

	void Entry() override;
	void Reave() override;

private:

	struct CBuff
	{
		float time;
		float dammy[3];
	};

	CBuff* m_cBuff;
	int m_cBuffH;
	int m_psH;
	int m_imgH;
	int m_dissolveH;

};
