#pragma once
#include "UIBase.h"

class Image;

class PlayerHPBar : public UIBase
{
public:
	PlayerHPBar();

	void Init();

	void Update() override;

	void Draw() const override;


private:

	// �o�[����邽�߂ɉ摜�𕡐�����
	std::shared_ptr<Image> m_baseImage;
	//std::shared_ptr<Image> m_fillImage;
};