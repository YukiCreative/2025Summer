#include "PlayerJumpSlash.h"

namespace
{
	constexpr int kEnableAttackFrame = 50;
	constexpr int kDisableAttackFrame = 62;
	constexpr int kStateWholeFrame = 168;
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
