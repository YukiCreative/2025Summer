#include "PlayerJumpSlash.h"
#include "Player.h"
#include "Collidable.h"
#include <DxLib.h>
#include "AnimationModel.h"
#include "Input.h"

namespace
{
	constexpr int kChargeFrame = 40;
	constexpr float kChargeForce = 5.0f;

	constexpr int kEnableAttackFrame = kChargeFrame;
	constexpr int kDisableAttackFrame = kChargeFrame + 10;
	constexpr int kStateWholeFrame = 168 + kChargeFrame;
	// �O�i����^�C�~���O
	constexpr int kTrackFrame = 5;
	// �O�i�����
	const float kTrackForce = 30.0f;
	// ���̍U�����͂̎�t�J�n����
	constexpr int kAcceptAttackInputFrame = 80;
	// �U�����h������^�C�~���O
	constexpr int kEnableComboFrame = 100;

	constexpr float kAttackPower = 150.0f;

	const std::string kAnimName = "Armature|JumpAttack";
	constexpr bool kIsLoopAnim = false;
}

PlayerJumpSlash::PlayerJumpSlash(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
}

PlayerJumpSlash::~PlayerJumpSlash()
{
}

void PlayerJumpSlash::Init()
{
	m_animName                   = kAnimName;
	m_isLoopAnim                 = kIsLoopAnim;
	m_enableAttackColFrame       = kEnableAttackFrame;
	m_disableAttackColFrame      = kDisableAttackFrame;
	m_stateTotalFrame            = kStateWholeFrame;
	m_trackFrame                 = kTrackFrame;
	m_trackForce                 = kTrackForce;
	m_acceptNextAttackInputFrame = kAcceptAttackInputFrame;
	m_enableComboFrame           = kEnableComboFrame;
	m_attackPower                = kAttackPower;
}

void PlayerJumpSlash::OptionalProcess()
{
	// �G�̈ʒu�Ɉړ�
	// �قړːi�U���ƕς��Ȃ�

	auto p = m_player.lock();

	if (m_frame == 10)
	{
		p->m_model->Stop();
	}

	// ��莞�ԑO�i
	// ���b�N�I�����Ă��镨�Ɍ�����
	if (m_frame <= kChargeFrame)
	{
		// �v���C���[����G�ւ�XZ�����̒P�ʃx�N�g��
		const Vector3 pToEXZN = (p->m_lockOnActor.lock()->GetPos().XZ() - p->GetPos().XZ()).GetNormalize();

		// �ːi
		p->GetCollidable().AddVel(VTransformSR({ 0,0,kChargeForce }, MGetRotVec2(Vector3::Foward(), pToEXZN)));
	}

	if (m_frame == kChargeFrame)
	{
		p->m_model->Play();
	}

	// �����ːi���Ƀ��b�N�I���ΏۂɐڐG������ːi���~�߂���
	if (p->m_isContactLockOnActor || Input::GetInstance().IsTrigger("Attack"))
	{
		// �ːi���I��钼�O�̃t���[�����Ƃ������Ƃɂ���
		// ���łɂ��̃��C���𒴂��Ă���Ή������Ȃ�
		m_frame = max(kChargeFrame - 1, m_frame);
	}
}
