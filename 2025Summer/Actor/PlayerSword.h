#pragma once
#include "Actor.h"
#include "Model.h"

class Player;

// モデルと当たり判定のセット
class PlayerSword : public Actor
{
public:
	PlayerSword();

	void Init(std::weak_ptr<Player> player);

	void Update() override;
	void Draw() const override;

	void OnCollision(const std::shared_ptr<Actor> other) override;

	void CommitMove() override;

	// プレイヤーがいじる
	// 武器出現
	void Enable();
	// 消去
	void Disable();

	void ColEnable();
	void ColDisable();
	void SetAttackPower(const float atk) { m_attackPower = atk; }

	// 敵が見る
	bool IsAttacking() const { return m_isAttacking; }
	float GetAttackPower() const { return m_attackPower; }

private:

	std::shared_ptr<Model> m_model;
	std::weak_ptr<Player> m_player;

	// プレイヤークラスが攻撃時にこの値を書き換える
	float m_attackPower;
	// 攻撃判定を出すか
	bool m_isAttacking;
	// モデルが出現しているかどうか
	bool m_isExisting;

	// モデルディゾルブ
	struct SwordCBuff
	{
		// 0～1
		float time;
		float dammy[3];
	};

	SwordCBuff* m_cBuff;
	int m_cBuffH;

private:

	void AppearUpdate();
	void DisappearUpdate();
};

