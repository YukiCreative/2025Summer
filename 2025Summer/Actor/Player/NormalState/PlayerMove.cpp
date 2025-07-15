#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "AnimationModel.h"
#include "PlayerSlashDown.h"
#include "Collidable.h"
#include <DxLib.h>
#include "PlayerLockOnIdle.h"

namespace
{
	constexpr float kRunSpeed = 0.0018f;
	constexpr int   kDashFrame = 120;

	const std::string kMoveAnimName = "Armature|RunDash";

	// �ړ��؂�̗�
	constexpr float kSashAttackForce = 30.0f;
}

PlayerMove::PlayerMove(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_moveFrame(0)
{
	// ���b�N�I����ԂȂ�A�j���[�V������؂�ւ��Ȃ�
	if (!m_player.lock()->m_lockOnActor.expired()) return;

	// �A�j���[�V������J��
	m_player.lock()->m_model->ChangeAnimation(kMoveAnimName);
}

PlayerMove::~PlayerMove()
{
}

std::shared_ptr<PlayerState> PlayerMove::Update()
{
	Input& input = Input::GetInstance();
	auto p = m_player.lock();

	// ���̏�ԂɑJ�ڂ������A�������b�N�I������Ă�����
	if (!m_player.lock()->m_lockOnActor.expired())
	{
		// ���b�N�I���̕��ɑJ�ڂ��Ă�����
		// ���Ǐ�������ɗ��邱�ƂɂȂ��
		// �܂��U����Ԃ𗼕��Ɏg���񂵂Ă�̂�������

		// �����Ɏl�����̈ړ���Ԃւ̑J�ڏ��������������Ȃ��̂ŁA��������LockOnIdle��ԂɈڍs����
		// ����ƍU����Idle��Move�Ɠ��A�j���[�V�������؂�ւ��̂ŁA�A�j���[�V�����⊮�����ʁB
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}

	// �ړ�
	p->Move(kRunSpeed);

	// ���͂��؂ꂽ��ҋ@��Ԃ�
	if (input.GetLeftInputAxis().SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	if (input.IsTrigger("Attack"))
	{
		// �ړ����ɍU������͂���Ƃ�����ƒǉ��őO�i����
		p->GetCollidable().AddVel(VTransformSR({0,0,kSashAttackForce}, p->GetModelMatrix()));

		return std::make_shared<PlayerSlashDown>(m_player);
	}

	++m_moveFrame;

	return shared_from_this();
}
