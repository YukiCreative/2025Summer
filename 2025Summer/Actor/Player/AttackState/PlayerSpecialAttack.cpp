#include "PlayerSpecialAttack.h"
#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"
#include "PlayerMove.h"
#include "PlayerIdle.h"

namespace
{
	// ������t���[��
	constexpr int kDisappearFrame = 40;
	constexpr int kAttackFrame = 150;
	constexpr int kAppearFrame = 200;
	constexpr int kStateWholeFrame = 360;
	// �ړ��Ȃǂɔh���ł���t���[��
	constexpr int kEnableTransitionFrame = 300;

	const std::string kStartAnimName = "Armature|SpecialAttack";
	const std::string kEndAnimName = "Armature|FrontStop";
	constexpr bool kIsLoopAnim = false;
}

PlayerSpecialAttack::PlayerSpecialAttack(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
	auto p = m_player.lock();

	// �A�j���[�V����
	p->ChangeAnim(kStartAnimName, kIsLoopAnim);
	// ���G
	p->SetInvincibility(true);

	p->SetCanLockOn(false);
	m_canCrossState = true;
}

PlayerSpecialAttack::~PlayerSpecialAttack()
{
	m_player.lock()->SetInvincibility(false);
}

std::shared_ptr<PlayerState> PlayerSpecialAttack::Update()
{
	auto p = m_player.lock();

	// ����̃t���[���ŏ�����
	if (m_frame == kDisappearFrame)
	{
		p->Disappear();
	}

	// ����̃t���[���ōU��
	if (m_frame == kAttackFrame)
	{
		p->SpecialAttack();
	}

	// �U�����I�������߂��Ă���A�j���[�V�����𗬂�
	if (m_frame == kAppearFrame)
	{
		p->Apeear();
		p->ChangeAnim(kEndAnimName, kIsLoopAnim);
	}

	// ���͂������Move
	if (m_frame > kEnableTransitionFrame)
	{
		if (Input::GetInstance().GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
		{
			return std::make_shared<PlayerMove>(m_player);
		}
	}

	// �A�j���[�V�������I����Idle��
	if (m_frame > kStateWholeFrame)
	{
		return std::make_shared<PlayerIdle>(m_player);
	}

	++m_frame;

	return shared_from_this();
}
