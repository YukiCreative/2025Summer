#include "PlayerJump.h"
#include "Player.h"
#include "PlayerFall.h"
#include "../../General/Input.h"
#include "PlayerLanding.h"
#include "AttackState/PlayerAirAttack1.h"
#include "../../Physics/Rigid.h"
#include "../../Geometry/Geometry.h"
#include "../../General/Game.h"

namespace
{
    const std::string kAnimName = "Armature|Jump";
    constexpr float kMoveSpeed = 0.0015f;
    constexpr float kJumpThreshold = 5.0f;
    constexpr float kLerpSpeed = 2.0f / static_cast<float>(Game::kFrameRate);
}

PlayerJump::PlayerJump(std::weak_ptr<Player> player) :
    PlayerState(player),
    m_frame(0),
    m_startPos(),
    m_targetPos()
{
    std::shared_ptr<Player> p = m_player.lock();

    m_startPos = p->GetPos();
    // ジャンプ
    // ロックオンしているかどうかで処理を分ける
    if (CheckLockOnJump())
    {
        Vector3 enemyPos = p->m_lockOnActor.lock()->GetPos();

        // 敵のほうがプレイヤーより一定以上高ければ
        // その高さに合うようにジャンプ
        m_targetPos = Vector3{ p->GetPos().x, enemyPos.y, p->GetPos().z };
    }
    else
    {
        // そうでないなら普通にジャンプ
        p->AddVel(kJumpForce);
        p->SetDragY(kStartDragY);
        m_targetPos = p->GetPos();
    }

    p->ChangeAnim(kAnimName, false);
}

PlayerJump::~PlayerJump()
{
    m_player.lock()->SetDragDefault();
}

std::shared_ptr<PlayerState> PlayerJump::Update()
{
    auto p = m_player.lock();
    Input& input = Input::GetInstance();

    if (m_frame == kSetDragDefaultFrame)
    {
        m_player.lock()->SetDragDefault();
    }
    // アニメーションが終わったらループモーションに
    if (p->IsAnimEnd())
    {
        return std::make_shared<PlayerFall>(m_player);
    }
    if (input.IsTrigger("Attack"))
    {
        return std::make_shared<PlayerAirAttack1>(m_player);
    }

    // ロックオンジャンプの処理
    if (CheckLockOnJump())
    {
        m_time += kLerpSpeed;
        m_time = std::min(m_time, 1.0f);
        p->SetPos(m_startPos.Lerp(m_targetPos, Geometry::Easing(m_time, Geometry::EasingKind::kOutQuart)));
        
        if (!CheckLockOnJump())
        {
            return std::make_shared<PlayerFall>(m_player);
        }
        p->CheckIsGround();
    }

    if (p->IsGround())
    {
        return std::make_shared<PlayerLanding>(m_player);
    }

    p->Move(kMoveSpeed);

    ++m_frame;

    return shared_from_this();
}

bool PlayerJump::CheckLockOnJump() const
{
    return m_player.lock()->IsLockOn() && m_player.lock()->m_lockOnActor.lock()->GetPos().y - m_player.lock()->GetPos().y > kJumpThreshold;
}
