#pragma once
#include "UIBase.h"
#include "RangeLimitedValue.h"

class Player;
class Image;

// ����Slider�݂����Ȓ��ۓI�ȃN���X�ɂ��Ă�������
class PlayerSpecialGauge : public UIBase
{
public:
	PlayerSpecialGauge();

	void Init(std::weak_ptr<Player> player);

	void Update() override;

	void Draw() const override;

private:

	// �o�[����邽�߂ɉ摜�𕡐�����
	std::shared_ptr<Image> m_baseImage;
	std::shared_ptr<Image> m_fillImage;

	Vector2 m_fillImageSize;

	using FillRatio_t = RangeLimitedValue<float, 0.0f, 1.0f>;
	FillRatio_t m_fillRatio;
	FillRatio_t m_targetFillRatio;

	std::weak_ptr<Player> m_player;
};

