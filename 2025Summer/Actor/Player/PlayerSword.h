#pragma once
#include "../AttackCol.h"
#include "../../Model/Model.h"
#include "../../Utility/RangeLimitedValue.h"
#include "../../GameManagement/Score/StylishEnum.h"

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

	void SetActionKind(const IncreaseStylishPointKind kind);

private:

	std::shared_ptr<Model> m_model;
	std::weak_ptr<Player> m_player;

	std::weak_ptr<EffekseerEffect> m_effect;

	// モデルが出現しているかどうか
	bool m_isExisting;

	// モデルディゾルブ
	struct SwordCBuff
	{
		Vector3 minPos;
		float dammy;
		Vector3 maxPos;
		// 0～1
		float time;
	};

	int m_psH;
	int m_vsH;
	int m_colTex;
	int m_dissolveTex;
	int m_frame;
	RangeLimitedValue<float, 0.0f, 1.0f> m_dissolveParam;
	SwordCBuff* m_cBuff;
	int m_cBuffH;

	IncreaseStylishPointKind m_actionKind;

private:

	void SetCBuffStatus();
};

