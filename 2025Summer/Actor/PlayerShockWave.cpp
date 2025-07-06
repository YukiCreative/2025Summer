#include "PlayerShockWave.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "Rigid.h"
#include <DxLib.h>

namespace
{
	constexpr float kMoveSpeed = 100.0f;

	constexpr float kColLength = 400.0f;
	constexpr float kColRadius = 50.0f;

	constexpr int kLifeTime = 40;
}

PlayerShockWave::PlayerShockWave() :
	AttackCol(),
	m_frame(0)
{
}

PlayerShockWave::~PlayerShockWave()
{
}

void PlayerShockWave::Init(const DxLib::tagMATRIX& rot, const Vector3& initPos, const float atk)
{
	m_kind = ActorKind::kPlayerAttack;

	// �R���C�_�[������
	auto col = std::make_shared<CapsuleCollider>();
	col->Init({ 0,-kColLength * 0.5f, 0 }, {0,kColLength * 0.5f, 0}, 0, true, true, kColRadius);
	// ��]�����Ă��畽�s�ړ�
	col->SetPos(VTransformSR(col->StartPos(), rot), VTransformSR(col->EndPos(), rot));
	col->SetPos(initPos);
	m_pos = col->GetPos();
	auto rigid = std::make_shared<Rigid>();
	rigid->Init({0,0,0});
	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_attackPower = atk;

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
		m_isAlive = false;
	}
	++m_frame;
}

void PlayerShockWave::Draw() const
{
	m_collidable->GetCol().Draw();
}

void PlayerShockWave::CommitMove()
{
	m_pos += m_collidable->UpdateRigid();

	m_collidable->SetPos(m_pos);
}

void PlayerShockWave::OnCollision(std::shared_ptr<Actor> other)
{
	// �G�ɓ������������
	if (other->GetKind() == ActorKind::kEnemy)
	{
		m_isAlive = false;
	}
}
