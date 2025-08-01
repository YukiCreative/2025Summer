#pragma once
#include "../PlayerState.h"
#include "../../../GameManagement/Score/StylishEnum.h"
#include <string>

class PlayerAttackState abstract : public PlayerState 
{
public:
	PlayerAttackState(std::weak_ptr<Player> parent);
	~PlayerAttackState();

	// コンボ先のインスタンスを持っている関係上、
	// 初期化のタイミングを任意にしないといけなくなった
	virtual void Init() abstract;
	void PlayAnim();

	std::shared_ptr<PlayerState> Update() override final;

protected:

	bool m_isEnterAttack;
	int m_frame;

	// これらの変数を派生クラスで初期化してもらう
	std::string m_animName;
	bool m_isLoopAnim;
	int m_enableAttackColFrame;
	int m_disableAttackColFrame;
	int m_stateTotalFrame;
	// 敵への吸いつき
	int m_trackFrame;
	float m_trackForce;
	float m_enemyTrackForce;
	int m_acceptNextAttackInputFrame;
	int m_enableComboFrame;
	float m_attackPower;
	float m_knockbackPower;
	// 派生する攻撃
	std::shared_ptr<PlayerAttackState> m_comboAttack;
	IncreaseStylishPointKind m_actionKind;


protected:

	// 派生先で任意の処理を実装したいときに使う
	virtual void OptionalProcess() {}

	// 敵に吸いつく挙動を実現します
	// 吸いつくときとそうでない(任意入力、対象なし)時で加える力を変えられる
	Vector3 TrackingVec(const float strength, const float trackEnemyStrength);
	Vector3 TrackVec(const float strength, const float trackEnemyStrength);
	// モデルを移動方向に回転
	// Trackから分けた
	void RotatePlayer(const Vector3& vec);
};

