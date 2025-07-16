#pragma once
#include "UIBase.h"
#include "RangeLimitedValue.h"

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

	// ���b�N�I���̉��o���֐��|�C���^�Ő��䂷��
	using LockOnCursorState_t = void (PlayerLockOnUI::*)();
	LockOnCursorState_t m_state;

	RangeLimitedValue<float, 0.0f, 1.0f> m_stateParam;

private:

	void StartLockOn();
	void EndLockOn();
	void DuringLockOn();
	void NoLockOn();

	void MoveToLockOnTarget();
};

