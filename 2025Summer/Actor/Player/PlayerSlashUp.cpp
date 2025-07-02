#include "PlayerSlashUp.h"
#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Input.h"
#include "Rigid.h"

namespace	
{
	constexpr int kEnableAttackFrame = 9;
	constexpr int kDisableAttackFrame = 20;
	constexpr int kStateWholeFrame = 78;
	// �O�i����^�C�~���O
	constexpr int kForwardFrame = 15;
	// �O�i�����
	const float kForwardForce = 30.0f;
	// ���̍U�����͂̎�t�J�n����
	constexpr int kAcceptAttackInputFrame = 10;
	// �U�����h������^�C�~���O
	constexpr int kEnableComboFrame = 26;

	constexpr float kAttackPower = 130.0f;

	const std::string kAnimName = "Armature|SlashUp";
}

PlayerSlashUp::PlayerSlashUp(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0),
	m_isEnterAttack(false)
{
	// �A�j���[�V����
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);
}

PlayerSlashUp::~PlayerSlashUp()
{
}

std::shared_ptr<PlayerState> PlayerSlashUp::Update()
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

	// �ҋ@��Ԃ֑J��
	if (m_frame >= kStateWholeFrame)
	{
		p->DiaableSword();

		return std::make_shared<PlayerIdle>(m_player);
	}

	++m_frame;

	return shared_from_this();
}
