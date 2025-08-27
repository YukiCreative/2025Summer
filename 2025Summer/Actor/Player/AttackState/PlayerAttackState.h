#pragma once
#include "../PlayerState.h"
#include "../../../GameManagement/Score/StylishEnum.h"
#include <string>

class PlayerAttackState abstract : public PlayerState 
{
public:
	PlayerAttackState(std::weak_ptr<Player> parent);
	~PlayerAttackState();

	// ƒRƒ“ƒ{æ‚ÌƒCƒ“ƒXƒ^ƒ“ƒX‚ğ‚Á‚Ä‚¢‚éŠÖŒWãA
	// ‰Šú‰»‚Ìƒ^ƒCƒ~ƒ“ƒO‚ğ”CˆÓ‚É‚µ‚È‚¢‚Æ‚¢‚¯‚È‚­‚È‚Á‚½
	virtual void Init() abstract;
	void PlayAnim();

	std::shared_ptr<PlayerState> Update() override final;

protected:

	bool m_isEnterAttack;
	int m_frame;

	// ‚±‚ê‚ç‚Ì•Ï”‚ğ”h¶ƒNƒ‰ƒX‚Å‰Šú‰»‚µ‚Ä‚à‚ç‚¤
	std::string m_animName;
	bool m_isLoopAnim;
	int m_enableAttackColFrame;
	int m_disableAttackColFrame;
	int m_stateTotalFrame;
	// “G‚Ö‚Ì‹z‚¢‚Â‚«
	int m_trackFrame;
	float m_trackForce;
	float m_enemyTrackForce;
	int m_acceptNextAttackInputFrame;
	int m_enableComboFrame;
	float m_attackPower;
	Vector3 m_knockbackPower;
	float m_stunPower;
	// ”h¶‚·‚éUŒ‚
	std::shared_ptr<PlayerAttackState> m_comboAttack;
	IncreaseStylishPointKind m_actionKind;
	bool m_isKnockUp = false;


protected:

	// ”h¶æ‚Å”CˆÓ‚Ìˆ—‚ğÀ‘•‚µ‚½‚¢‚Æ‚«‚Ég‚¤
	virtual std::shared_ptr<PlayerState> OptionalProcess() { return shared_from_this(); }

	// “G‚É‹z‚¢‚Â‚­‹““®‚ğÀŒ»‚µ‚Ü‚·
	// ‹z‚¢‚Â‚­‚Æ‚«‚Æ‚»‚¤‚Å‚È‚¢(”CˆÓ“ü—ÍA‘ÎÛ‚È‚µ)‚Å‰Á‚¦‚é—Í‚ğ•Ï‚¦‚ç‚ê‚é
	Vector3 TrackingVec(const float strength, const float trackEnemyStrength);
	Vector3 TrackVec(const float strength, const float trackEnemyStrength);
	// ƒ‚ƒfƒ‹‚ğˆÚ“®•ûŒü‚É‰ñ“]
	// Track‚©‚ç•ª‚¯‚½
	void RotatePlayer(const Vector3& vec);
};

