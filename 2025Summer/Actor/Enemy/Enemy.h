#pragma once
#include "Actor.h"
#include <string>
#include "RangeLimitedValue.h"
#include "HitPoint.h"

class AnimationModel;
class EnemyState;
class Player;
class AttackCol;

// 敵の基底処理
class Enemy abstract : public Actor
{
public:
	// ロックオンの設定
	Enemy();

	void Init(std::weak_ptr<Player> player, const Vector3& initPos, const float initHP);

	// ここでは純粋仮想関数は継承しない

	void ChangeAnim(const std::string& animName, const bool isLoop = true);

	void LookAtPlayer();
	Vector3 EnemyToPlayer() const;

	virtual void OnDeath() abstract;

protected:

	HitPoint_t m_hitPoint;
	// モデルは共通で持っている
	std::shared_ptr<AnimationModel> m_model;
	// プレイヤーの参照を持たせる
	std::weak_ptr<Player> m_player;
	bool m_isInvincible;

protected:

	virtual void OnDamage(std::shared_ptr<AttackCol> other) abstract;

private:
};

