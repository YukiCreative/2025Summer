#include "PlayerShockWave.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "Rigid.h"
#include "Player.h"
#include <DxLib.h>

namespace
{
	constexpr float kMoveSpeed = 100.0f;

	constexpr float kColLength = 400.0f;
	constexpr float kColRadius = 50.0f;

	constexpr int kLifeTime = 40;
	constexpr int kResidueFrame = 1;

	constexpr float kDrag = 0.0f;

	constexpr int kChargeGaugeBasePoint = 1;
	// �^�_���[�W�̂��̕������K�E�Z�𑝉�
	constexpr float kSpecialAttackAttackPowerMult = 0.01f;
}

PlayerShockWave::PlayerShockWave() :
	AttackCol(),
	m_frame(0)
{
}

PlayerShockWave::~PlayerShockWave()
{
}

void PlayerShockWave::Init(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk, const float knockback,
	std::weak_ptr<Player> player)
{
	m_kind = ActorKind::kPlayerAttack;

	m_player = player;

	// �R���C�_�[������
	auto col = std::make_shared<CapsuleCollider>();
	col->Init({ 0,-kColLength * 0.5f, 0 }, {0,kColLength * 0.5f, 0}, 0, true, true, kColRadius);
	// ��]�����Ă��畽�s�ړ�
	col->SetPos(VTransformSR(col->StartPos(), rot), VTransformSR(col->EndPos(), rot));
	col->SetPos(initPos);
	m_pos = col->GetPos();
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kDrag);
	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_attackPower = atk;
	m_knockbackPower = knockback;

	// ����ŃR�s�[��shared_ptr�Ƃ��č����
	m_dir = std::make_shared<MATRIX>(rot);
}

void PlayerShockWave::Update()
{
	// ��葬�x�ňړ�
	m_collidable->SetVel(VTransformSR({ 0,0,kMoveSpeed }, *m_dir));

	// ��莞�Ԃŏ�����
	if (m_frame >= kLifeTime)
	{
		Destroy();
	}
	++m_frame;
}

void PlayerShockWave::Draw() const
{
#if _DEBUG
	m_collidable->GetCol().Draw();
#endif
}

void PlayerShockWave::CommitMove()
{
	m_pos += m_collidable->UpdateRigid();

	m_collidable->SetPos(m_pos);
}

void PlayerShockWave::OnCollisionEnter(std::shared_ptr<Actor> other)
{
	// �G�ɓ������������
	if (other->GetKind() == ActorKind::kEnemy)
	{
		// ������Ƃ����P�\���������āA�����̂̓G�ɓ�����悤�ɂ���

		m_frame = kLifeTime - kResidueFrame;

		// �Q�[�W����
		m_player.lock()->ChargeSpecialGauge(kChargeGaugeBasePoint + static_cast<int>(m_attackPower * kSpecialAttackAttackPowerMult));
	}
}
