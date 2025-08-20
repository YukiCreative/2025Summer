#include "PlayerJump.h"
#include "Player.h"
#include "PlayerFall.h"

namespace
{
    const Vector3 kJumpForce = {0.0f, 30.0f, 0.0f};
    const std::string kAnimName = "Armature|Jump";
}

PlayerJump::PlayerJump(std::weak_ptr<Player> player) :
    PlayerState(player),
    m_frame(0)
{
    // ƒWƒƒƒ“ƒv
    m_player.lock()->AddVel(kJumpForce);
    m_player.lock()->ChangeAnim(kAnimName, false);
}

PlayerJump::~PlayerJump()
{
}

std::shared_ptr<PlayerState> PlayerJump::Update()
{
    auto p = m_player.lock();

    if (p->IsAnimEnd())
    {
        return std::make_shared<PlayerFall>(m_player);
    }

    return shared_from_this();
}
