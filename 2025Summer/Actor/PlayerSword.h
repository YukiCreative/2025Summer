#pragma once
#include "Actor.h"
#include "Model.h"

class Player;

// ���f���Ɠ����蔻��̃Z�b�g
class PlayerSword : public Actor
{
public:
	PlayerSword();

	void Init(std::weak_ptr<Player> player);

	void Update() override;
	void Draw() const override;

	void OnCollision(const std::shared_ptr<Actor> other) override;

	void CommitMove() override;

	// �v���C���[��������
	// ����o��
	void Enable();
	// ����
	void Disable();

	void ColEnable();
	void ColDisable();
	void SetAttackPower(const float atk) { m_attackPower = atk; }

	// �G������
	bool IsAttacking() const { return m_isAttacking; }
	float GetAttackPower() const { return m_attackPower; }

private:

	std::shared_ptr<Model> m_model;
	std::weak_ptr<Player> m_player;

	// �v���C���[�N���X���U�����ɂ��̒l������������
	float m_attackPower;
	// �U��������o����
	bool m_isAttacking;
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

