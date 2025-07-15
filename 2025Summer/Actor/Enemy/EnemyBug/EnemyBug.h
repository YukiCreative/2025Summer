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

	// �X�e�[�g�ɂ��ϐ����B���Ă݂邩
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
	// �U���Ԋu�̎擾�Ɏg�����K���z�̗���������
	// �݂�ȂŎg���܂킷�̂�static
	static std::normal_distribution<> s_attackTimeNormalDist;
	// �U���J�ڂ͌��̃N���X�ł��
	// �����̃X�e�[�g���܂����ł��������p���������̂�
	int m_attackFrame;

private:

	void OnDamage(std::shared_ptr<AttackCol> other);
};

