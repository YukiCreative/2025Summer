#pragma once
#include "AttackCol.h"
#include "Model.h"

class Player;
class EffekseerEffect;

// モデルと当たり判定のセット
class PlayerSword : public AttackCol
{
public:
	PlayerSword();
	~PlayerSword();

	void Init(std::weak_ptr<Player> player);

	void Update() override;
	void Draw() const override;

	void OnCollisionEnter(const std::shared_ptr<Actor> other) override;

	void CommitMove() override;

	// プレイヤーがいじる
	// 武器出現
	void Enable();
	// 消去
	void Disable();

	void ColEnable();
	void ColDisable();

private:

	std::shared_ptr<Model> m_model;
	std::weak_ptr<Player> m_player;

	std::weak_ptr<EffekseerEffect> m_effect;

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

