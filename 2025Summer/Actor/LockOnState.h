#pragma once

// ����(6/21)�A���b�N�I���ł��邩�ǂ����̂ݎ���
// ���̃Q�[���Ǝ��̎d�l�ŁAActor�ɒǉ��������Ȃ��ϐ��Ƃ��͂�����
class LockOnState
{
public:
	LockOnState(const bool canLockOn) :
		m_canLockOn(canLockOn)
	{
	}

	const bool CanLockOn() const { return m_canLockOn; }
	void SetCanLockOn(const bool canLockOnOrNot) { m_canLockOn = canLockOnOrNot; }

private:

	bool m_canLockOn;
};