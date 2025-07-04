#include "PlayerAttackState.h"
#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "Rigid.h"
#include "ActorController.h"
#include "Geometry.h"
#include "Camera.h"
#include "AnimationModel.h"

PlayerAttackState::PlayerAttackState(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_isEnterAttack(false),
	m_frame(0),
	m_comboAttack(nullptr)
{
}

PlayerAttackState::~PlayerAttackState()
{
}

void PlayerAttackState::PlayAnim()
{
	m_player.lock()->m_model->ChangeAnimation(m_animName, m_isLoopAnim);
}

std::shared_ptr<PlayerState> PlayerAttackState::Update()
{
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	// Œ•‚ÌUŒ‚”»’è‚ð—LŒø‰»
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

	// æs‚µ‚Ä“ü—Í‚ð‚Æ‚Á‚Ä‚¨‚­
	if (m_frame >= m_acceptNextAttackInputFrame)
	{
		m_isEnterAttack |= input.IsTrigger("Attack");
	}

	// ŽŸ‚ÌUŒ‚‚Ö
	if (m_frame >= m_enableComboFrame && m_isEnterAttack)
	{
		// ”h¶æ‚ªÝ’è‚³‚ê‚Ä‚¢‚ê‚Î
		if (m_comboAttack)
		{
			// ‰Šú‰»‚µ‚Ä•Ô‚·
			m_comboAttack->Init();
			m_comboAttack->PlayAnim();
			return m_comboAttack;
		}
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

Vector3 PlayerAttackState::TrackingVec(const float strength)
{
	Vector3 vel;
	// “ü—Í‚ª‚ ‚Á‚½‚çA‚»‚Ì•ûŒü‚É“®‚­
	const auto& inputAxis = Input::GetInstance().GetLeftInputAxis();
	auto p = m_player.lock();

	if (inputAxis.SqrMagnitude() > kMoveThreshold)
	{
		// “ü—Í‚ðƒJƒƒ‰‚É‰ñ“]
		Vector3 inputV = p->m_camera.lock()->RotateVecToCameraDirXZ({ -inputAxis.x, 0, inputAxis.y, }, Vector3::Back());

		vel = inputV.GetNormalize() * strength;
	}
	else
	{
		// ‚»‚¤‚Å‚È‚¯‚ê‚Î‹ß‚­‚Ì“G‚Ì•ûŒü
		auto nearestActor = p->m_cont.lock()->GetNearestLockOnActor(m_player.lock()->GetPos());
		auto posToNearest = nearestActor->GetPos() - p->GetPos();
		if (posToNearest.SqrMagnitude() < 100000)
		{
			vel = posToNearest.GetNormalize() * strength;
		}
		else
		{
			// ‚³‚ç‚Éˆê’è”ÍˆÍ“à‚É“G‚à‚¢‚È‚©‚Á‚½‚çŒ»Ý‚Ìƒ‚ƒfƒ‹‚ÌŒü‚«‚É‘Oi
			vel = VTransformSR({ 0,0,strength }, m_player.lock()->GetModelMatrix());
		}
	}

	auto c = Geometry::Corner(p->m_model->GetDirection(), vel.XZ());

	// •„†‚ð‚Â‚¯‚é
	if (p->m_model->GetDirection().Cross(vel.XZ()).y < 0)
	{
		c *= -1;
	}

	// ƒ‚ƒfƒ‹‚ð‰ñ“]‚³‚¹‚é
	p->m_model->RotateUpVecY(c);

	return vel;
}