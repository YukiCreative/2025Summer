#include "AnimationModel.h"
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
    const Vector3 kLockOnCameraPosOffsetLeft = { -100, 200, -300 };
    const Vector3 kLockOnCameraPosOffsetRight = { 100, 200, -300 };

    const Vector3 kLockOnLineStartOffset = { 0, 200, 0 };

    const Vector3 kPlayerMiddlePointOffset = { 0, 100, 0 };

    constexpr float kRotateSpeed = 0.2f;
    constexpr float kCameraRotSpeed = 0.005f;
    constexpr float kChangeDistanceSpeed = 10.0f;
}

PlayerLockOn::PlayerLockOn(std::weak_ptr<Player> parent) :
    PlayerState(parent)
{
    auto p = m_player.lock();

    // 初期状態
    m_childState = std::make_shared<PlayerLockOnIdle>(m_player);

    // 攻撃判定を消しておく
    p->DisableSwordCol();
    p->DisableSword();

    // コマンドリストを初期化
    p->m_inputList.clear();
}

PlayerLockOn::~PlayerLockOn()
{
}

std::shared_ptr<PlayerState> PlayerLockOn::Update()
{
    auto& input = Input::GetInstance();
    auto p = m_player.lock();
    auto camera = p->m_camera.lock();

    // ロックオンが外れたら、通常状態へ
    if (m_player.lock()->m_lockOnActor.expired())
    {
        return std::make_shared<PlayerNormal>(m_player);
    }

    // 通常のカメラ回転
    p->CameraMove();

    // targetPosを更新
    p->m_targetPos = (p->GetPos() + kLockOnLineStartOffset + p->m_lockOnActor.lock()->GetPos()) * 0.5f;

    // プレイヤーが画面外に出たら
    // その向きにカメラ回転
    auto playerScreenPos = ConvWorldPosToScreenPos(p->GetPos() + kPlayerMiddlePointOffset);
    auto targetScreenPos = ConvWorldPosToScreenPos(camera->GetTargetPos());

    // 条件複雑だな
    if (playerScreenPos.x > Game::kScreenWidth * 0.7f && playerScreenPos.z > 0 && playerScreenPos.z < targetScreenPos.z)
    {
        p->m_camera.lock()->RotateCameraUpVecY(-kCameraRotSpeed);
    }
    if (playerScreenPos.x < Game::kScreenWidth * 0.3f && playerScreenPos.z > 0 && playerScreenPos.z < targetScreenPos.z)
    {
        p->m_camera.lock()->RotateCameraUpVecY(kCameraRotSpeed);
    }
    auto cameraToPlayer = p->GetPos(); GetCameraPosition();

    // カメラの回転円の外側にプレイヤーや敵が出そうなら
    // カメラの回転半径を伸ばす
    auto targetToPlayerXZ = camera->GetTargetPos().XZ() - p->GetPos().XZ();

    if (targetToPlayerXZ.SqrMagnitude() > (camera->GetTargetDistance() - 300) * (camera->GetTargetDistance() - 300))
    {
        camera->SetTargetDistance(camera->GetTargetDistance() + kChangeDistanceSpeed);
    }
    else if (camera->GetTargetDistance() > 400.0f)
    {
        camera->SetTargetDistance(camera->GetTargetDistance() + -kChangeDistanceSpeed);
    }

    // プレイヤーを敵方向に回転
    auto lockOnPosXZ = p->m_lockOnActor.lock()->GetPos().XZ();
    auto posXZ = p->GetPos().XZ();

    auto playerToLockOnXZ = (lockOnPosXZ - posXZ).GetNormalize();

    auto playerDir = p->m_model->GetDirection();

    auto dot = playerToLockOnXZ.Dot(playerDir);

    float rot = playerDir.Cross(playerToLockOnXZ).y * kRotateSpeed;

    // ちょうど真反対に向いていた場合の処理
    if (dot < -0.9999f && rot < 0.0001f)
    {
        rot += 0.1f;
    }

    p->m_model->RotateUpVecY(rot);

    Vector3 inputAxis = Vector3{ input.GetLeftInputAxis().x, 0, input.GetLeftInputAxis().y };
    inputAxis.z *= -1;
    Vector3 cameraRotatedAxis = p->m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Foward());

    // 状態をUpdate
    m_childState = m_childState->Update();

    // ロックオンボタンを離したら
    if (Input::GetInstance().IsReleased("LockOn"))
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
