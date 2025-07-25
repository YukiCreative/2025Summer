#pragma once

// 現状(6/21)、ロックオンできるかどうかのみ実装
// このゲーム独自の仕様で、Actorに追加したくない変数とかはここに
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