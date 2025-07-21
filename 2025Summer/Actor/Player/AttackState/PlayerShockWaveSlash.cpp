#include "PlayerShockWaveSlash.h"
#include "Player.h"
#include "Collidable.h"
#include <DxLib.h>
#include "Geometry.h"

namespace
{
	constexpr int kEnableAttackFrame = 51;
	constexpr int kDisableAttackFrame = 57;

	// ���̍U���͓��U�����肪����
	constexpr int kEnableAttackFrame2 = 69;
	constexpr int kDisableAttackFrame2 = 76;

	// �Ռ��g�^�C�~���O
	constexpr int kShockWaveFrame1 = 53;
	constexpr int kShockWaveFrame2 = 71; 

	constexpr int kStateWholeFrame = 132;
	// �O�i����^�C�~���O
	constexpr int kTrackFrame = 51;
	constexpr int kTrackFrame2 = 69;
	// �O�i�����
	const float kTrackForce = 20.0f;
	// ���̍U�����͂̎�t�J�n����
	constexpr int kAcceptAttackInputFrame = 80;
	// �U�����h������^�C�~���O
	constexpr int kEnableComboFrame = 120;

	constexpr float kAttackPower = 50.0f;
	constexpr float kAttackPower2 = 60.0f;

	const std::string kAnimName = "Armature|Drive";
	constexpr bool kIsLoopAnim = false;

	constexpr float kShockWaveRot1 = 80.0f;
	constexpr float kShockWaveRot2 = -80.0f;
	constexpr float kShockWaveAtk1 = 130.0f;
	constexpr float kShockWaveAtk2 = 150.0f;
	constexpr float kKnockbackPower1 = 10.0f;
	constexpr float kKnockbackPower2 = 30.0f;
}

PlayerShockWaveSlash::PlayerShockWaveSlash(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
	// ��i�ڂȂ̂Ŏ����ŏ���������
	Init();
	PlayAnim();
}

PlayerShockWaveSlash::~PlayerShockWaveSlash()
{
}

void PlayerShockWaveSlash::Init()
{
	// ���o��
	auto p = m_player.lock();
	p->EnableSword();

	// ���̑������͋��ʉ��ł��Ȃ����낤��
	// �O���f�[�^�����Ď擾����Ή�������
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

	// ���ڂ̑O�i
	if (m_frame == kTrackFrame2)
	{
		p->GetCollidable().AddVel(TrackingVec(kTrackForce, kTrackForce));
	}

	// ����̃t���[���ŁA�Ռ��g�����
	if (m_frame == kShockWaveFrame1)
	{
		auto rot = MMult(MGetRotZ(kShockWaveRot1 * Geometry::kDegToRad), MGetRotElem(p->GetModelMatrix()));
		p->SpawnShockWave(rot, p->GetRightInexPos(), kShockWaveAtk1, kKnockbackPower1);
	}
	if (m_frame == kShockWaveFrame2)
	{
		auto rot = MMult(MGetRotZ(kShockWaveRot2 * Geometry::kDegToRad), MGetRotElem(p->GetModelMatrix()));
		p->SpawnShockWave(rot, p->GetRightInexPos(), kShockWaveAtk2, kKnockbackPower2);
	}
}
