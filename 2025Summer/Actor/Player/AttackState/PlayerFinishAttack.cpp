#include "PlayerFinishAttack.h"
#include "PlayerSlashDown.h"

namespace
{
	const std::string kAnimName = "Armature|FinishAttack";
	constexpr bool kIsLoopAnim = false;

	constexpr int kEnableAttackFrame = 46;
	constexpr int kDisableAttackFrame = 56;
	constexpr int kStateWholeFrame = 132;
	// �O�i����^�C�~���O
	constexpr int kTrackFrame = 0;
	// �O�i�����
	const float kTrackForce = 20.0f;
	// ���̍U�����͂̎�t�J�n����
	constexpr int kAcceptAttackInputFrame = 60;
	// �U�����h������^�C�~���O
	constexpr int kEnableComboFrame = 70;

	constexpr float kAttackPower = 400.0f;
	constexpr float kKnockbackPower = 50.0f;
}

PlayerFinishAttack::PlayerFinishAttack(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
}

PlayerFinishAttack::~PlayerFinishAttack()
{
}

void PlayerFinishAttack::Init()
{
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
	m_comboAttack = std::make_shared<PlayerSlashDown>(m_player);
}
