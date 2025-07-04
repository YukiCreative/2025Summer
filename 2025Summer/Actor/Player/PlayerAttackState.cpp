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

	// Œ•‚ÌUŒ‚”»’è‚ğ—LŒø‰»
	if (m_frame == m_enableAttackColFrame)
	{
		p->EnableSwordCol(m_attackPower);
	}
	if (m_frame == m_disableAttackColFrame)
	{
		p->DisableSwordCol();
	}

	// “G‚É‹z‚¢‚Â‚­‹““®or‘Oi
	if (m_frame == m_trackFrame)
	{
		p->GetRigid().AddVel(TrackingVec(m_trackForce));
	}

	// æs‚µ‚Ä“ü—Í‚ğ‚Æ‚Á‚Ä‚¨‚­
	if (m_frame >= m_acceptNextAttackInputFrame)
	{
		m_isEnterAttack |= input.IsTrigger("Attack");
	}

	// Ÿ‚ÌUŒ‚‚Ö
	if (m_frame >= m_enableComboFrame && m_isEnterAttack)
	{
		// ‰Šú‰»‚µ‚Ä•Ô‚·
		m_comboAttack->Init();
		return m_comboAttack;
	}

	// ‘Ò‹@ó‘Ô‚Ö‘JˆÚ
	if (m_frame >= m_stateTotalFrame)
	{
		p->DiaableSword();

		return std::make_shared<PlayerIdle>(m_player);
	}

	// ”CˆÓ‚Ìˆ—
	OptionalProcess();

	++m_frame;

	return shared_from_this();
}
