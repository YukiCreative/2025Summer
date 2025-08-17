#pragma once

// このゲーム独自の仕様で、Actorに追加したくない変数とかはここに
class ActorAttribute
{
public:
	ActorAttribute(const bool canLockOn) :
		m_canLockOn(canLockOn),
		m_isPosLimited(true)
	{
	}

	const bool CanLockOn() const { return m_canLockOn; }
	void SetCanLockOn(const bool canLockOnOrNot) { m_canLockOn = canLockOnOrNot; }

	const bool IsPosLimited() const { return m_isPosLimited; }
	void SetPosLimit(const bool limitedOrNot) { m_isPosLimited = limitedOrNot; }

private:

	bool m_canLockOn;
	// 仮の仕様なので必要なくなったら消す
	bool m_isPosLimited;
};