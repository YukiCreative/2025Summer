#include "PlayerLockOnUI.h"
#include "Player.h"
#include "Image.h"

namespace
{
	const std::string kImageName = "LockOnCursor.png";
}

PlayerLockOnUI::PlayerLockOnUI()
{
}

void PlayerLockOnUI::Init(std::weak_ptr<Player> player)
{
	m_player = player;

	m_image = std::make_shared<Image>();
	m_image->Init(kImageName);
}

void PlayerLockOnUI::Update()
{
	// ���b�N�I������Ă���Ώۂ̃X�N���[����̈ʒu�Ɉړ�
	auto lockOnActorScreenPos = m_player.lock()->GetLockOnActorScreenPos();

	// Z�͕ω����Ăق����Ȃ�
	m_pos.x = lockOnActorScreenPos.x;
	m_pos.y = lockOnActorScreenPos.y;
}

void PlayerLockOnUI::Draw() const
{
	if (!m_player.lock()->IsLockOn()) return;

	m_image->Draw({ m_pos.x, m_pos.y });
}
