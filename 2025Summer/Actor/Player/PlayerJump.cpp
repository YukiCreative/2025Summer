#include "PlayerJump.h"
#include "Player.h"
#include "PlayerFall.h"
#include "../../General/Input.h"
#include "PlayerLanding.h"
#include "AttackState/PlayerAirAttack1.h"
#include "../../Physics/Rigid.h"

namespace
{
    const std::string kAnimName = "Armature|Jump";
    constexpr float kMoveSpeed = 0.0015f;
    constexpr float kJumpThreshold = 50.0f;
    constexpr float kLerpSpeed = 0.05f;
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
    if (p->IsLockOn())
    {
        Vector3 enemyPos = p->m_lockOnActor.lock()->GetPos();

        // 敵のほうがプレイヤーより一定以上高ければ
        if (p->GetPos().y - enemyPos.y > kJumpThreshold)
        {
            // その高さに合うようにジャンプ
            m_targetPos = Vector3{ p->GetPos().x, enemyPos.y, p->GetPos().z };
        }
    }
    else
    {
        // そうでないなら普通にジャンプ
        p->AddVel(kJumpForce);
        p->SetDragY(kStartDragY);
    }

    p->ChangeAnim(kAnimName, false);
}

PlayerJump::~PlayerJump()
{
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
    if (p->IsGround())
    {
        return std::make_shared<PlayerLanding>(m_player);
    }

    // ロックオンジャンプの処理
    if (p->IsLockOn() && p->GetPos().y - p->m_lockOnActor.lock()->GetPos().y > kJumpThreshold)
    {
        p->SetPos(m_startPos.Lerp(m_targetPos, m_time));
        m_time += kLerpSpeed;
        m_time = std::min(m_time, 1.0f);
        if (p->GetPos().y - p->m_lockOnActor.lock()->GetPos().y < kJumpThreshold)
        {
            return std::make_shared<PlayerFall>(m_player);
        }
    }

    p->Move(kMoveSpeed);

    ++m_frame;

    return shared_from_this();
}
