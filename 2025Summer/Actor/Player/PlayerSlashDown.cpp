#include "PlayerSlashDown.h"
#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Input.h"
//#include "PlayerSlashUp.h"
//#include "PlayerSlashSide.h"
#include "PlayerSlashLeftSide.h"
#include "Actorcontroller.h"
#include "Rigid.h"

namespace
{
	constexpr int kEnableAttackFrame = 10;
	constexpr int kDisableAttackFrame = 25;
	constexpr int kStateWholeFrame = 58;
	// �O�i����^�C�~���O
	constexpr int kForwardFrame = 10;
	// �O�i�����
	const float kForwardForce = -20.0f;
	// ���̍U�����͂̎�t�J�n����
	constexpr int kAcceptAttackInputFrame = 10;
	// �U�����h������^�C�~���O
	constexpr int kEnableComboFrame = 26;

	constexpr float kAttackPower = 100.0f;

	const std::string kAnimName = "Armature|SlashDown";
}

PlayerSlashDown::PlayerSlashDown(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_isEnterAttack(false),
	m_frame(0)
{
	// ��i�ڂȂ̂Ō����o��
	m_player.lock()->EnableSword();

	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);
}

PlayerSlashDown::~PlayerSlashDown()
{
}

std::shared_ptr<PlayerState> PlayerSlashDown::Update()
{
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	// ���̍U�������L����
	if (m_frame == kEnableAttackFrame)
	{
		p->EnableSwordCol(kAttackPower);
	}
	if (m_frame == kDisableAttackFrame)
	{
		p->DisableSwordCol();
	}

	// �U�������u�Ԉړ�
	if (m_frame == kForwardFrame)
	{
		p->GetRigid().AddVel(TrackingVec(kForwardForce));
	}

	// ��s���ē��͂��Ƃ��Ă���
	if (m_frame >= kAcceptAttackInputFrame)
	{
		m_isEnterAttack |= input.IsTrigger("Attack");
	}

	// ���̍U����
	if (m_frame >= kEnableComboFrame && m_isEnterAttack)
	{
		return std::make_shared<PlayerSlashLeftSide>(m_player);
	}

	// �ҋ@��Ԃ֑J��
	if (m_frame >= kStateWholeFrame)
	{
		p->DiaableSword();

		return std::make_shared<PlayerIdle>(m_player);
	}

	++m_frame;

	return shared_from_this();
}
