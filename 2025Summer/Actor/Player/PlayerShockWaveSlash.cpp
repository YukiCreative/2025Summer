#include "PlayerShockWaveSlash.h"
#include "Player.h"

namespace
{
	constexpr int kEnableAttackFrame = 54;
	constexpr int kDisableAttackFrame = 62;

	// ���̍U���͓��U�����肪����
	constexpr int kEnableAttackFrame2 = 84;
	constexpr int kDisableAttackFrame2 = 92;

	// �Ռ��g�^�C�~���O
	constexpr int kShockWaveFrame1 = 58;
	constexpr int kShockWaveFrame2 = 88;

	constexpr int kStateWholeFrame = 140;
	// �O�i����^�C�~���O
	constexpr int kTrackFrame = 20;
	// �O�i�����
	const float kTrackForce = 20.0f;
	// ���̍U�����͂̎�t�J�n����
	constexpr int kAcceptAttackInputFrame = 80;
	// �U�����h������^�C�~���O
	constexpr int kEnableComboFrame = 120;

	constexpr float kAttackPower = 50.0f;
	constexpr float kAttackPower2 = 60.0f;

	constexpr float kShockWavePower = 100.0f;
	constexpr float kShockWavePower2 = 120.0f;

	const std::string kAnimName = "Armature|Drive";
	constexpr bool kIsLoopAnim = false;
}

PlayerShockWaveSlash::PlayerShockWaveSlash(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
}

PlayerShockWaveSlash::~PlayerShockWaveSlash()
{
}

void PlayerShockWaveSlash::Init()
{
	// ���̑������͋��ʉ��ł��Ȃ����낤��
	// �O���f�[�^�����Ď擾����Ή�������
	m_animName = kAnimName;
	m_isLoopAnim = kIsLoopAnim;
	m_enableAttackColFrame = kEnableAttackFrame;
	m_disableAttackColFrame = kDisableAttackFrame;
	m_stateTotalFrame = kStateWholeFrame;
	m_trackFrame = kTrackFrame;
	m_trackForce = kTrackForce;
	m_acceptNextAttackInputFrame = kAcceptAttackInputFrame;
	m_enableComboFrame = kEnableComboFrame;
	m_attackPower = kAttackPower;
}

void PlayerShockWaveSlash::OptionalProcess()
{
	auto p = m_player.lock();

	// ���ڂ̍U������
	if (m_frame == kEnableAttackFrame2)
	{
		p->EnableSwordCol(kAttackPower2);
	}
	if (m_frame == kDisableAttackFrame2)
	{
		p->DisableSwordCol();
	}

	// ����̃t���[���ŁA�Ռ��g�����
	if (m_frame == kShockWaveFrame1)
	{
		
	}
	if (m_frame == kShockWaveFrame2)
	{

	}
}
