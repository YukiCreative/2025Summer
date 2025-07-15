#pragma once
#include "AttackCol.h"
#include "Model.h"

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

private:

	std::shared_ptr<Model> m_model;
	std::weak_ptr<Player> m_player;

	std::weak_ptr<EffekseerEffect> m_effect;

	// ���f�����o�����Ă��邩�ǂ���
	bool m_isExisting;

	// ���f���f�B�]���u
	struct SwordCBuff
	{
		// 0�`1
		float time;
		float dammy[3];
	};

	SwordCBuff* m_cBuff;
	int m_cBuffH;

private:

	void AppearUpdate();
	void DisappearUpdate();
};

