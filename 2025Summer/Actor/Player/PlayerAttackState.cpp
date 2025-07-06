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

Vector3 PlayerAttackState::TrackingVec(const float strength)
{
	Vector3 vel;
	// ���͂���������A���̕����ɓ���
	const auto& inputAxis = Input::GetInstance().GetLeftInputAxis();
	auto p = m_player.lock();

	// ���b�N�I������Ă��炸�A���͂������
	if (inputAxis.SqrMagnitude() > kMoveThreshold && p->m_lockOnActor.expired())
	{
		// ���͂��J�����ɉ�]
		Vector3 inputV = p->m_camera.lock()->RotateVecToCameraDirXZ({ -inputAxis.x, 0, inputAxis.y, }, Vector3::Back());

		vel = inputV.GetNormalize() * strength;
	}
	else
	{
		// �����łȂ���΋߂��̓G�̕���
		auto nearestActor = p->m_cont.lock()->GetNearestLockOnActor(m_player.lock()->GetPos());
		auto posToNearest = nearestActor->GetPos() - p->GetPos();
		if (posToNearest.SqrMagnitude() < 100000)
		{
			vel = posToNearest.GetNormalize() * strength;
		}
		else
		{
			// ����Ɉ��͈͓��ɓG�����Ȃ������猻�݂̃��f���̌����ɑO�i
			vel = VTransformSR({ 0,0,strength }, m_player.lock()->GetModelMatrix());
		}
	}

	auto c = Geometry::Corner(p->m_model->GetDirection(), vel.XZ());

	// ����������
	if (p->m_model->GetDirection().Cross(vel.XZ()).y < 0)
	{
		c *= -1;
	}

	// ���f������]������
	p->m_model->RotateUpVecY(c);

	return vel;
}