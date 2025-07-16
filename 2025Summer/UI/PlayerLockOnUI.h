#pragma once
#include "UIBase.h"

class Image;
class Player;

// ���b�N�I�������Ƃ��ɕ\�������A���ł�
class PlayerLockOnUI : public UIBase
{
public:
	PlayerLockOnUI();

	void Init(std::weak_ptr<Player> player);

	void Update() override;
	void Draw() const override;

private:

	std::shared_ptr<Image> m_image;
	std::weak_ptr<Player> m_player;
};

