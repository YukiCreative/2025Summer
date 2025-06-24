#include "Camera.h"
#include "Game.h"
#include "Input.h"
#include "Player.h"
#include "PlayerLockOn.h"
#include "PlayerLockOnIdle.h"
#include "PlayerNormal.h"
#include <DxLib.h>

namespace
{
    const Vector3 kLockOnCameraPosOffsetLeft = { -100, 200, -200 };
    const Vector3 kLockOnCameraPosOffsetRight = { 100, 200, -200 };

    const Vector3 kLockOnLineStartOffset = { 0, 200, 0 };
}

PlayerLockOn::PlayerLockOn(std::weak_ptr<Player> parent) :
    PlayerState(parent)
{
    // 初期状態
    m_childState = std::make_shared<PlayerLockOnIdle>(m_player);

    // カメラ
    m_player.lock()->m_camera.lock()->ChangeStateDP();
}

PlayerLockOn::~PlayerLockOn()
{
}

std::shared_ptr<PlayerState> PlayerLockOn::Update()
{
    // ロックオンが外れたら、通常状態へ
    if (m_player.lock()->m_lockOnActor.expired())
    {
        return std::make_shared<PlayerNormal>(m_player);
    }

    CameraMove();

    // 状態をUpdate
    m_childState = m_childState->Update();

    // ここにロックオン関連の処理を入れる
    // ロックオンボタンを押したら近くの敵をロックオン
    if (Input::GetInstance().IsTrigger("LockOn"))
    {
        ReleaseLockOn();

        // 次のUpdateでNormal状態へ移行
    }

    return shared_from_this();
}

void PlayerLockOn::ReleaseLockOn()
{
    // 解除
    m_player.lock()->m_lockOnActor.reset();

    // いじったカメラを元に戻す
    m_player.lock()->m_camera.lock()->ChangeStateDD();
}

void PlayerLockOn::CameraMove()
{
    // プレイヤーとロックオン対象が画面外に出ないようにしたい
    auto p = m_player.lock();
    auto cam = p->m_camera.lock();

    // カメラの位置を決めてしまう

    // 常にロックオン対象→プレイヤー→カメラ　にする

    const Vector3 playerPos = p->GetPos();

    const Vector3 playerToLock = p->m_lockOnActor.lock()->GetPos().XZ() - playerPos.XZ();
    auto mat = MGetRotVec2(Vector3::Foward(), playerToLock);

    // プレイヤーが画面右側か左側か
    if (ConvWorldPosToScreenPos(playerPos).x < Game::kScreenHalfWidth)
    {
        // 左側にプレイヤー→カメラは右側に
        cam->SetLerpPos(playerPos + VTransformSR(kLockOnCameraPosOffsetRight, mat));
    }
    else
    {
        // 逆もまた然り
        cam->SetLerpPos(playerPos + VTransformSR(kLockOnCameraPosOffsetLeft, mat));
    }

    // targetPosを更新
    p->m_targetPos = (playerPos + kLockOnLineStartOffset + p->m_lockOnActor.lock()->GetPos()) * 0.5f;
}
