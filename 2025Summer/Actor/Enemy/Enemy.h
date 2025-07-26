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
	int m_cBuffH;
	EnemyCBuff* m_cBuff;
	// 甘んじてすべてのEnemyでロードしよう
	int m_dissolvePsH;
	int m_dissolveVsH;
	int m_dissolveTex;

	bool m_isDissolving;

	std::weak_ptr<EffekseerEffect> m_bloodEffect;
	// 派生クラスで設定してもらう
	int m_bloodFrame;

protected:

	virtual void OnDamage(std::shared_ptr<AttackCol>) abstract;

private:

	void InitDissolve();
	// 死亡演出Draw
	void UpdateDissolve();
	void DissolveDraw() const;

};