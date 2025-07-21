#include "PlayerSpecialAttack.h"

namespace
{
	constexpr int kEnableAttackFrame = 9;
	constexpr int kDisableAttackFrame = 20;
	constexpr int kStateWholeFrame = 78;
	// �O�i����^�C�~���O
	constexpr int kTrackFrame = 12;
	// �O�i�����
	const float kTrackForce = 10.0f;
	// ���̍U�����͂̎�t�J�n����
	constexpr int kAcceptAttackInputFrame = 30;
	// �U�����h������^�C�~���O
	constexpr int kEnableComboFrame = 50;

	constexpr float kAttackPower = 130.0f;
	constexpr float kKnockbackPower = 30.0f;

	const std::string kAnimName = "Armature|SlashUp";
	constexpr bool kIsLoopAnim = false;
}

PlayerSpecialAttack::PlayerSpecialAttack(std::weak_ptr<Player> parent) :
	PlayerAttackState(parent)
{
}

PlayerSpecialAttack::~PlayerSpecialAttack()
{
}

void PlayerSpecialAttack::Init()
{
	// �Ƃ肠��������̍U���Ō��ɓ����蔻���t����͎̂^��
}

void PlayerSpecialAttack::OptionalProcess()
{

}
