#pragma once
#include "Actor.h"
#include <string>

class AnimationModel;
class EnemyState;
class Player;

// 敵の基底処理
class Enemy abstract : public Actor
{
public:
	// ロックオンの設定
	Enemy();

	void Init(std::weak_ptr<Player> player, const Vector3& initPos);

	// ここでは純粋仮想関数は継承しない

	void ChangeAnim(const std::string& animName, const bool isLoop = true);

	void LookAtPlayer();
	Vector3 EnemyToPlayer() const;

protected:

	// モデルは共通で持っている
	std::shared_ptr<AnimationModel> m_model;

	// プレイヤーの参照を持たせる
	std::weak_ptr<Player> m_player;

private:
};

