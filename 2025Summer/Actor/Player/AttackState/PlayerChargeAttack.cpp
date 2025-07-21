#include "PlayerChargeAttack.h"
#include "Player.h"
#include "Collidable.h"
#include <DxLib.h>
#include "AnimationModel.h"
#include "Input.h"

namespace
{
	// ���̃t���[���܂őO�i
	constexpr int kChargeFrame = 30;
	constexpr float kChargeForce = 5.0f;

	constexpr int kEnableAttackFrame = kChargeFrame;
	constexpr int kDisableAttackFrame = kChargeFrame + 10;
	constexpr int kStateWholeFrame = 60 + kChargeFrame;
	// �O�i����^�C�~���O
	constexpr int kTrackFrame = 5;
	// �O�i�����
	const float kTrackForce = 30.0f;
	// ���̍U�����͂̎�t�J�n����
	constexpr int kAcceptAttackInputFrame = 30;
	// �U�����h������^�C�~���O
	constexpr int kEnableComboFrame = 50 + kChargeFrame;

	constexpr float kAttackPower = 2000.0f;
	constexpr float kKnockbackPower = 40.0f;

	const std::string kAnimName = "Armature|LeftHorizontalSlash";
	constexpr bool kIsLoopAnim = false;
}

PlayerChargeAttack::PlayerChargeAttack(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
	// ��i�ڂȂ̂Ŏ����ŏ���������
	Init();
	PlayAnim();
}

PlayerChargeAttack::~PlayerChargeAttack()
{
}

void PlayerChargeAttack::Init()
{
	m_animName = kAnimName;
	m_isLoopAnim = kIsLoopAnim;
	m_enableAttackColFrame = kEnableAttackFrame;
	m_disableAttackColFrame = kDisableAttackFrame;
	m_stateTotalFrame = kStateWholeFrame;
	m_trackFrame = kTrackFrame;
	m_trackForce = kTrackForce;
	m_enemyTrackForce = kTrackForce;
	m_acceptNextAttackInputFrame = kAcceptAttackInputFrame;
	m_enableComboFrame = kEnableComboFrame;
	m_attackPower = kAttackPower;
	m_knockbackPower = kKnockbackPower;

	// ���o��
	auto p = m_player.lock();
	p->EnableSword();
}

void PlayerChargeAttack::OptionalProcess()
{
	auto p = m_player.lock();

	if (m_frame == 10)
	{
		p->m_model->Stop();
	}

	// ��莞�ԑO�i
	// ���b�N�I�����Ă��镨�Ɍ�����
	// ���b�N�I�����ĂȂ���΂��̂܂ܑO�i
	if (m_frame <= kChargeFrame)
	{
		if (p->IsLockOn())
		{
			// �v���C���[����G�ւ�XZ�����̒P�ʃx�N�g��
			const Vector3 pToEXZN = (p->m_lockOnActor.lock()->GetPos().XZ() - p->GetPos().XZ()).GetNormalize();

			// �ːi
			p->GetCollidable().AddVel(VTransformSR({ 0,0,kChargeForce }, MGetRotVec2(Vector3::Foward(), pToEXZN)));
		}
		else
		{
			// �O�i
			p->GetCollidable().AddVel(VTransformSR({ 0,0,kChargeForce }, p->GetModelMatrix()));
		}
	}

	if (m_frame == kChargeFrame)
	{
		p->m_model->Play();
	}

	// �����̕ύX�͏�ʂ̃X�e�[�g�ł��łɍs���Ă���̂ł��Ȃ�

	// �����ːi���Ƀ��b�N�I���ΏۂɐڐG������ːi���~�߂���
	if (p->m_isContactLockOnActor || Input::GetInstance().IsTrigger("Attack"))
	{
		// �ːi���I��钼�O�̃t���[�����Ƃ������Ƃɂ���
		// ���łɂ��̃��C���𒴂��Ă���Ή������Ȃ�
		m_frame = std::max(kChargeFrame-1, m_frame);
	}
}
