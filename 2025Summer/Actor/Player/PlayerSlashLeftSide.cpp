#include "PlayerSlashLeftSide.h"

#include "Player.h"
#include "AnimationModel.h"
#include "PlayerIdle.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Input.h"
#include "PlayerSlashUp.h"

namespace
{
	constexpr int kEnableAttackFrame = 10;
	constexpr int kDisableAttackFrame = 16;
	constexpr int kStateWholeFrame = 78;
	// �O�i����^�C�~���O
	constexpr int kForwardFrame = 12;
	// �O�i�����
	const Vector3 kForwardVel = { 0, 0, -20.0f };
	// ���̍U�����͂̎�t�J�n����
	constexpr int kAcceptAttackInputFrame = 10;
	// �U�����h������^�C�~���O
	constexpr int kEnableComboFrame = 26;

	constexpr float kAttackPower = 100.0f;

	const std::string kAnimName = "Armature|LeftHorizontalSlash";
}

PlayerSlashLeftSide::PlayerSlashLeftSide(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0),
	m_isEnterAttack(false)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);
}

PlayerSlashLeftSide::~PlayerSlashLeftSide()
{
}

std::shared_ptr<PlayerState> PlayerSlashLeftSide::Update()
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
		// ���͂���������A���̕����ɓ���
		// �����łȂ���΋߂��̓G�̕���
		// ����Ɉ��͈͓��ɓG�����Ȃ������猻�݂̃��f���̌����ɑO�i

				// ���͂���������A���̕����ɓ���
		const auto& inputAxis = input.GetLeftInputAxis();

		Vector3 vel;

		if (inputAxis.SqrMagnitude() > kMoveThreshold)
		{
			vel = VTransformSR(kForwardVel, MGetRotVec2(kForwardVel, { inputAxis.x, 0, inputAxis.y, }));
		}

		p->GetCollidable().AddVel(VTransformSR(kForwardVel, p->GetModelMatrix()));
	}

	// ��s���ē��͂��Ƃ��Ă���
	if (m_frame >= kAcceptAttackInputFrame)
	{
		m_isEnterAttack |= input.IsTrigger("Attack");
	}

	// ���̍U����
	if (m_frame >= kEnableComboFrame && m_isEnterAttack)
	{
		return std::make_shared<PlayerSlashUp>(m_player);
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
