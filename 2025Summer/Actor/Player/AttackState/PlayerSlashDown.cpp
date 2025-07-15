#include "PlayerSlashDown.h"
#include "PlayerSlashLeftSide.h"
#include "Player.h"

namespace
{
	const std::string kAnimName = "Armature|SlashDown";
	constexpr bool kIsLoopAnim = false;

	constexpr int kEnableAttackFrame = 10;
	constexpr int kDisableAttackFrame = 25;
	constexpr int kStateWholeFrame = 58;
	// �O�i����^�C�~���O
	constexpr int kTrackFrame = 0;
	// �O�i�����
	const float kTrackForce = 20.0f;
	// ���̍U�����͂̎�t�J�n����
	constexpr int kAcceptAttackInputFrame = 10;
	// �U�����h������^�C�~���O
	constexpr int kEnableComboFrame = 26;

	constexpr float kAttackPower = 100.0f;
	constexpr float kKnockbackPower = 10.0f;
}

PlayerSlashDown::PlayerSlashDown(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
	// ��i�ڂȂ̂Ŏ����ŏ���������
	Init();
	PlayAnim();
}

PlayerSlashDown::~PlayerSlashDown()
{
}


void PlayerSlashDown::Init()
{
	// ��i�ڂȂ̂Ō����o��
	m_player.lock()->EnableSword();

	m_animName = kAnimName;
	m_isLoopAnim = kIsLoopAnim;
	m_enableAttackColFrame = kEnableAttackFrame;
	m_disableAttackColFrame = kDisableAttackFrame;
	m_stateTotalFrame = kStateWholeFrame;
	m_trackFrame = kTrackFrame;
	m_trackForce = kTrackForce;
	m_enemyTrackForce = kTrackForce * 3.0f;
	m_acceptNextAttackInputFrame = kAcceptAttackInputFrame;
	m_enableComboFrame = kEnableComboFrame;
	m_attackPower = kAttackPower;
	m_knockbackPower = kKnockbackPower;
	m_comboAttack = std::make_shared<PlayerSlashLeftSide>(m_player);
}