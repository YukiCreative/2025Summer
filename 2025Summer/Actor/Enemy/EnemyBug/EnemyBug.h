#pragma once
#include "Enemy.h"
#include <random>

class AnimationModel;
class EnemyBugState;
class AttackCol;

namespace DxLib
{
	struct tagMATRIX;
}

class EnemyBug : public Enemy, public std::enable_shared_from_this<EnemyBug>
{
public:
	EnemyBug();

	void Init(const std::weak_ptr<Player> player, const Vector3& initPos);

	void Update() override;
	void Draw() const override;

	void CommitMove() override;

	void OnCollisionEnter(std::shared_ptr<Actor> other) override;
	void OnCollisionStay(std::shared_ptr<Actor> other) override;
	void OnCollisionExit(std::shared_ptr<Actor> other) override;

	void OnDeath();

	// ステートにも変数を隠してみるか
	std::weak_ptr<Player> GetPlayer() const { return m_player; }
	void GenerateAttackCol();
	int GetAinmTotalTime() const;

	Vector3 GetAttackRigPos() const;

	int GetAttackInterval();

	int GetAttackFrame() const { return m_attackFrame; }
	void SetAttackFrame(const int frame) { m_attackFrame = frame; }
	void CountAttackFrame() { --m_attackFrame; }

private:

	std::shared_ptr<EnemyBugState> m_state;
	// 攻撃間隔の取得に使う正規分布の乱数発生器
	// みんなで使いまわすのでstatic
	static std::normal_distribution<> s_attackTimeNormalDist;
	// 攻撃遷移は元のクラスでやる
	// 複数のステートをまたいでも処理を継続したいので
	int m_attackFrame;

private:

	void OnDamage(std::shared_ptr<AttackCol> other);
};

