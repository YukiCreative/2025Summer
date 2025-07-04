#pragma once
#include "PlayerState.h"
#include <string>

class PlayerAttackState : public PlayerState
{
public:
	PlayerAttackState(std::weak_ptr<Player> parent);
	~PlayerAttackState();

	// コンボ先のインスタンスを持っている関係上、
	// 初期化のタイミングを任意にしないといけなくなった
	virtual void Init() abstract;

	std::shared_ptr<PlayerState> Update() override;

private:

	bool m_isEnterAttack;
	int m_frame;

	// これらの変数を派生クラスで初期化してもらう
	std::string m_animName;
	int m_enableAttackColFrame;
	int m_disableAttackColFrame;
	int m_stateTotalFrame;
	// 敵への吸いつき
	int m_trackFrame;
	float m_trackForce;
	int m_acceptNextAttackInputFrame;
	int m_enableComboFrame;
	float m_attackPower;
	// 派生する攻撃
	std::shared_ptr<PlayerAttackState> m_comboAttack;

private:

	// 派生先で任意の処理を実装したいときに使う
	virtual void OptionalProcess() {}
};

