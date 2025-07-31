#pragma once
#include "../AttackCol.h"
#include "../../Model/Model.h"
#include "../../Utility/RangeLimitedValue.h"
#include "../../GameManagement/Score/StylishEnum.h"

class Player;
class EffekseerEffect;

// ���f���Ɠ����蔻��̃Z�b�g
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

	// �v���C���[��������
	// ����o��
	void Enable();
	// ����
	void Disable();

	void ColEnable();
	void ColDisable();

	void SetActionKind(const IncreaseStylishPointKind kind);

private:

	std::shared_ptr<Model> m_model;
	std::weak_ptr<Player> m_player;

	std::weak_ptr<EffekseerEffect> m_effect;

	// ���f�����o�����Ă��邩�ǂ���
	bool m_isExisting;

	// ���f���f�B�]���u
	struct SwordCBuff
	{
		Vector3 minPos;
		float dammy;
		Vector3 maxPos;
		// 0�`1
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

