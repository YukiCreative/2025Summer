#pragma once
#include "UIBase.h"
#include "../Geometry/Vector2.h"
#include "../Utility/RangeLimitedValue.h"

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

	// バーを作るために画像を複数持つ
	std::shared_ptr<Image> m_baseImage;
	std::shared_ptr<Image> m_fillImage;

	Vector2 m_fillImageSize;

	using FillRatio_t = RangeLimitedValue<float, 0.0f, 1.0f>;
	FillRatio_t m_fillRatio;
	FillRatio_t m_targetFillRatio;

	std::weak_ptr<Player> m_player;
};