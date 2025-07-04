#include "PlayerAttackState.h"
#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "Rigid.h"

PlayerAttackState::PlayerAttackState(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
}

PlayerAttackState::~PlayerAttackState()
{
}

std::shared_ptr<PlayerState> PlayerAttackState::Update()
{
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	// ���̍U�������L����
	if (m_frame == m_enableAttackColFrame)
	{
		p->EnableSwordCol(m_attackPower);
	}
	if (m_frame == m_disableAttackColFrame)
	{
		p->DisableSwordCol();
	}

	// �G�ɋz��������or�O�i
	if (m_frame == m_trackFrame)
	{
		p->GetRigid().AddVel(TrackingVec(m_trackForce));
	}

	// ��s���ē��͂��Ƃ��Ă���
	if (m_frame >= m_acceptNextAttackInputFrame)
	{
		m_isEnterAttack |= input.IsTrigger("Attack");
	}

	// ���̍U����
	if (m_frame >= m_enableComboFrame && m_isEnterAttack)
	{
		// ���������ĕԂ�
		m_comboAttack->Init();
		return m_comboAttack;
	}

	// �ҋ@��Ԃ֑J��
	if (m_frame >= m_stateTotalFrame)
	{
		p->DiaableSword();

		return std::make_shared<PlayerIdle>(m_player);
	}

	// �C�ӂ̏���
	OptionalProcess();

	++m_frame;

	return shared_from_this();
}
