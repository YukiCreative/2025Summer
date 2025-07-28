#pragma once
#include "Actor.h"
#include <string>
#include "RangeLimitedValue.h"
#include "HitPoint.h"
#include "EnemyKind.h"

class AnimationModel;
class EnemyState;
class Player;
class AttackCol;

namespace DxLib
{
	struct tagMATRIX;
}

class EffekseerEffect;

// 敵の基底処理
class Enemy abstract : public Actor
{
public:
	// ロックオンの設定
	Enemy();
	~Enemy();

	void Init(std::weak_ptr<Player> player, const Vector3& initPos, const float initHP, const int dupulicatedHandle);

	void Update() override final;
	virtual void UpdateState() abstract;

	void Draw() const override final;

	void ChangeAnim(const std::string& animName, const bool isLoop = true);

	void LookAtPlayer(const float strength = 1.0f);
	Vector3 EnemyToPlayer() const;
	// これはモデルが初期状態でZ-方向に向いている前提です
	Vector3 GetDir() const;
	void RotateAxisY(const float rad);
	DxLib::tagMATRIX GetModelMatrix() const;
	// trueで無敵
	void SetInvincibility(const bool invincibleOrNot) { m_isInvincible = invincibleOrNot; }
	bool IsEndAnim() const;
	void AddVel(const Vector3& vel);

	virtual void OnDeath() abstract;
	void StartBloodEffect();

	EnemyKind GetEnemyKind() const { return m_enemyKind; }

	// 自身をロックオン不可にする
	void DisableLockOn();

protected:

	// モデルディゾルブ
	struct EnemyCBuff
	{
		// 0～1
		float time;
		float dammy[7];
	};

	HitPoint_t m_hitPoint;
	// モデルは共通で持っている
	std::shared_ptr<AnimationModel> m_model;
	EnemyKind m_enemyKind;
	// プレイヤーの参照を持たせる
	std::weak_ptr<Player> m_player;
	bool m_isInvincible;

	std::weak_ptr<EffekseerEffect> m_bloodEffect;

	// 死亡時の血しぶきエフェクトの発生源をどのリグにするか
	// 派生クラスで設定してもらう
	int m_bloodFrameIndex;

protected:

	virtual void OnDamage(std::shared_ptr<AttackCol>) abstract;

private:

};