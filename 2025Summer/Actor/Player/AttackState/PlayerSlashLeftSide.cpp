#include "PlayerSlashLeftSide.h"

#include "../Player.h"
#include "../../../Model/AnimationModel.h"
#include "../NormalState/PlayerIdle.h"
#include "../../../Physics/Collidable.h"
#include <DxLib.h>
#include "../../../General/Input.h"
#include "PlayerSlashUp.h"
#include "../../../Physics/Rigid.h"

namespace
{
	constexpr int kEnableAttackFrame = 10;
	constexpr int kDisableAttackFrame = 16;
	constexpr int kStateWholeFrame = 78;
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

	const std::string kAnimName = "Armature|LeftHorizontalSlash";
	constexpr bool kIsLoopAnim = false;
}

PlayerSlashLeftSide::PlayerSlashLeftSide(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
}

PlayerSlashLeftSide::~PlayerSlashLeftSide()
{
}

void PlayerSlashLeftSide::Init()
{
	m_actionKind = IncreaseStylishPointKind::kSlashLeftSide;
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
	m_comboAttack = std::make_shared<PlayerSlashUp>(m_player);
}