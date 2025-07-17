#pragma once
#include "UIBase.h"
#include "Vector2.h"
#include "RangeLimitedValue.h"

class Image;
class Player;

class PlayerHPBar : public UIBase
{
public:
	PlayerHPBar();

	void Init(std::weak_ptr<Player> player);

	void Update() override;

	void Draw() const override;

private:

	// ƒo[‚ğì‚é‚½‚ß‚É‰æ‘œ‚ğ•¡”‚Â
	std::shared_ptr<Image> m_baseImage;
	std::shared_ptr<Image> m_fillImage;

	Vector2 m_fillImageSize;

	using FillRatio_t = RangeLimitedValue<float, 0.0f, 1.0f>;
	FillRatio_t m_fillRatio;
	FillRatio_t m_targetFillRatio;

	std::weak_ptr<Player> m_player;
};