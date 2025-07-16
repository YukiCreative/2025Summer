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

	// ƒo[‚ğì‚é‚½‚ß‚É‰æ‘œ‚ğ•¡”‚Â
	std::shared_ptr<Image> m_baseImage;
	//std::shared_ptr<Image> m_fillImage;
};