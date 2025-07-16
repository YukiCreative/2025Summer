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
	// ロックオンされている対象のスクリーン上の位置に移動
	auto lockOnActorScreenPos = m_player.lock()->GetLockOnActorScreenPos();

	// Zは変化してほしくない
	m_pos.x = lockOnActorScreenPos.x;
	m_pos.y = lockOnActorScreenPos.y;
}

void PlayerLockOnUI::Draw() const
{
	if (!m_player.lock()->IsLockOn()) return;

	m_image->Draw({ m_pos.x, m_pos.y });
}
