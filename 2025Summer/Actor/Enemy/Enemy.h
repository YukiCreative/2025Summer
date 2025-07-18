#pragma once
#include "Actor.h"
#include <string>
#include "RangeLimitedValue.h"
#include "HitPoint.h"

class AnimationModel;
class EnemyState;
class Player;
class AttackCol;

namespace DxLib
{
	struct tagMATRIX;
}

// 敵の基底処理
class Enemy abstract : public Actor
{
public:
	// ロックオンの設定
	Enemy();

	void Init(std::weak_ptr<Player> player, const Vector3& initPos, const float initHP, const int dupulicatedHandle);

	// ここでは純粋仮想関数は継承しない

	void ChangeAnim(const std::string& animName, const bool isLoop = true);

	void LookAtPlayer(const float strength = 1.0f);
	Vector3 EnemyToPlayer() const;
	// これはモデルが初期状態でZ-方向に向いている前提です
	Vector3 GetDir() const;
	DxLib::tagMATRIX GetModelMatrix() const;
	void SetInvincibility(const bool invincibleOrNot) { m_isInvincible = invincibleOrNot; }
	bool IsEndAnim() const;
	void AddVel(const Vector3& vel);

protected:

	HitPoint_t m_hitPoint;
	// モデルは共通で持っている
	std::shared_ptr<AnimationModel> m_model;
	// プレイヤーの参照を持たせる
	std::weak_ptr<Player> m_player;
	bool m_isInvincible;

protected:

private:
};

