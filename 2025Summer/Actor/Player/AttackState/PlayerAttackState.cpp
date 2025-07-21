#include "PlayerAttackState.h"
#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "Rigid.h"
#include "ActorController.h"
#include "Geometry.h"
#include "Camera.h"
#include "AnimationModel.h"
#include "PlayerMove.h"

namespace
{ 
	constexpr float kTrackEnemyDistance = 500.0f;
}

PlayerAttackState::PlayerAttackState(std::weak_ptr<Player> parent) :  
    PlayerState(parent),  
    m_isEnterAttack(false),  
    m_frame(0),  
    m_comboAttack(nullptr),  
    m_acceptNextAttackInputFrame(0),  
    m_attackPower(0.0f),  
    m_disableAttackColFrame(0),  
    m_enableAttackColFrame(0),  
    m_enableComboFrame(0),  
    m_enemyTrackForce(0.0f),  
    m_isLoopAnim(false),  
    m_knockbackPower(0.0f),  
    m_stateTotalFrame(0),  
    m_trackForce(0.0f),  
    m_trackFrame(0)  
{  
	// ���b�N�I�����܂���
	m_canCrossState = true;
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

	// ���̍U�������L����
	if (m_frame == m_enableAttackColFrame)
	{
		p->EnableSwordCol(m_attackPower, m_knockbackPower);
	}
	if (m_frame == m_disableAttackColFrame)
	{
		p->DisableSwordCol();
	}

	// �G�ɋz��������or�O�i
	if (m_frame == m_trackFrame)
	{
		p->GetRigid().AddVel(TrackingVec(m_trackForce, m_enemyTrackForce));
	}

	// ��s���ē��͂��Ƃ��Ă���
	if (m_frame >= m_acceptNextAttackInputFrame)
	{
		m_isEnterAttack |= input.IsTrigger("Attack");
	}

	// ���̍U����
	// �l�X�g�[����
	if (m_frame >= m_enableComboFrame)
	{
		if (m_isEnterAttack)
		{
			// �h���悪�ݒ肳��Ă����
			if (m_comboAttack)
			{
				// ���������ĕԂ�
				m_comboAttack->Init();
				m_comboAttack->PlayAnim();
				return m_comboAttack;
			}
		}

		// �ړ����Ă�����
		if (input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
		{
			p->DisableSword();

			// �ړ���Ԃֈڍs
			// ���b�N�I����Ԃł�����ɓ��삷��悤�ɂȂ��Ă���̂ŐS�z�Ȃ�
			return std::make_shared<PlayerMove>(m_player);
		}
	}

	// �ҋ@��Ԃ֑J��
	if (m_frame >= m_stateTotalFrame)
	{
		p->DisableSword();

		return std::make_shared<PlayerIdle>(m_player);
	}

	// �C�ӂ̏���
	OptionalProcess();

	++m_frame;

	return shared_from_this();
}

Vector3 PlayerAttackState::TrackingVec(const float strength, const float trackEnemyStrength)
{
	Vector3 vel;

	vel = TrackVec(strength, trackEnemyStrength);

	RotatePlayer(vel);

	return vel;
}

Vector3 PlayerAttackState::TrackVec(const float strength, const float trackEnemyStrength)
{
	const auto& inputAxis = Input::GetInstance().GetLeftInputAxis();
	auto p = m_player.lock();

	// ���͂������
	if (inputAxis.SqrMagnitude() > kMoveThreshold && p->m_lockOnActor.expired())
	{
		Vector3 inputV = p->m_camera.lock()->RotateVecToCameraDirXZ({ -inputAxis.x, 0, inputAxis.y, }, Vector3::Back());
		// ���͂̌����ɑO�i
		return inputV.GetNormalize() * strength;
	}
		
	// ���b�N�I�����Ă�Ȃ�A���̓G�̕���
	if (p->IsLockOn())
	{
		return (p->m_lockOnActor.lock()->GetPos() - p->GetPos()).GetNormalize() * trackEnemyStrength;
	}

	auto nearestActor = p->m_cont.lock()->GetNearestLockOnActor(m_player.lock()->GetPos());
	// �����łȂ���΋߂��̓G�̕���
	if (nearestActor && (nearestActor->GetPos() - p->GetPos()).SqrMagnitude() < kTrackEnemyDistance * kTrackEnemyDistance)
	{
		return (nearestActor->GetPos() - p->GetPos()).GetNormalize() * trackEnemyStrength;
	}

	// ����Ɉ��͈͓��ɓG�����Ȃ������猻�݂̃��f���̌����ɑO�i
	return VTransformSR({ 0,0,strength }, m_player.lock()->GetModelMatrix());
}

void PlayerAttackState::RotatePlayer(const Vector3& vec)
{
	auto p = m_player.lock();

	auto c = Geometry::Corner(p->m_model->GetDirection(), vec.XZ());

	// ����������
	if (p->m_model->GetDirection().Cross(vec.XZ()).y < 0)
	{
		c *= -1;
	}

	// ���f������]������
	p->m_model->RotateUpVecY(c);
}
