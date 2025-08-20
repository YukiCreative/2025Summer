#include "PlayerJump.h"
#include "Player.h"
#include "PlayerFall.h"
#include "../../General/Input.h"
#include "AttackState/PlayerAirAttack1.h"

namespace
{
    const Vector3 kJumpForce = {0.0f, 30.0f, 0.0f};
    const std::string kAnimName = "Armature|Jump";
    constexpr float kMoveSpeed = 0.0015f;
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
    Input& input = Input::GetInstance();

    if (p->IsAnimEnd())
    {
        return std::make_shared<PlayerFall>(m_player);
    }
    if (input.IsTrigger("Attack"))
    {
        return std::make_shared<PlayerAirAttack1>(m_player);
    }

    p->Move(kMoveSpeed);

    return shared_from_this();
}
