#include "../../../Model/AnimationModel.h"
#include "../../../Camera/Camera.h"
#include "../../../General/Game.h"
#include "../../../General/Input.h"
#include "../Player.h"
#include "PlayerLockOn.h"
#include "PlayerLockOnIdle.h"
#include "../NormalState/PlayerNormal.h"
#include "../AttackState/PlayerSpecialAttack.h"
#include <DxLib.h>
#include <algorithm>

namespace
{
    const Vector3 kLockOnCameraPosOffsetLeft = { -100, 200, -300 };
    const Vector3 kLockOnCameraPosOffsetRight = { 100, 200, -300 };

    const Vector3 kLockOnLineStartOffset = { 0, 200, 0 };

    const Vector3 kPlayerMiddlePointOffset = { 0, 100, 0 };

    constexpr float kCameraRotSpeed = 0.0001f;
    constexpr float kCameraRotSpeedMax = 30.0f;
    constexpr float kChangeDistanceSpeed = 10.0f;
    // プレイヤーがカメラに収まるためのXZ平面上の距離
    // カメラはこの距離分回転半径を伸ばす
    constexpr float kPlayerIntoCameraXZOffset = 300.0f;
    constexpr float kMoveTargetPosSpeed = 0.01f;

    constexpr float kDefaultCameraDistance = 400.0f;

    // スクリーン横方向のプレイヤーを収めようとする範囲
    constexpr float kScreenFitThresholdX = 0.2f;
    constexpr float kScreenFitThresholdY = 0.1f;

    constexpr float kCameraAutoRotateDepthOffset = 200.0f;

    constexpr float kGetAwayTargetPosScreenHeightThreshold = 0.9f;
    constexpr float kStopMoveTargetPosScreenHeightThreshold = 0.8f;

    // プレイヤー側 0～1 エネミー側
    constexpr float kMiddleTargetPos = 0.6f;
}

PlayerLockOn::PlayerLockOn(std::weak_ptr<Player> parent, std::shared_ptr<PlayerState> initState) :
    PlayerIntermediateState(parent),
    m_targetPosLerpParam(0.5f)
{
    auto p = m_player.lock();

    // ロックオン状態をまたがせたいステートなら続行
    if (initState->CanCrossState())
    {
        m_childState = initState;
    }
    else
    {
        m_childState = std::make_shared<PlayerLockOnIdle>(parent);
    }

    // 攻撃判定を消しておく
    p->DisableSwordCol();
    p->DisableSword();

    // コマンドリストを初期化
    p->m_inputList.clear();

    // カメラが急に移動しないようにする
    Vector3 nextTargetPos = (p->GetPos() + p->m_lockOnActor.lock()->GetPos()) * 0.5f + kLockOnLineStartOffset;
    Vector3 cPos = GetCameraPosition();
    auto nextTargetDistance = (nextTargetPos - cPos).Magnitude();
    p->m_camera.lock()->SetTargetDistance(nextTargetDistance);
}

PlayerLockOn::~PlayerLockOn()
{
}

std::shared_ptr<PlayerIntermediateState> PlayerLockOn::Update()
{
    auto& input = Input::GetInstance();
    auto p = m_player.lock();
    auto camera = p->m_camera.lock();

    // ロックオンが外れたら、通常状態へ
    if (m_player.lock()->m_lockOnActor.expired())
    {
        return std::make_shared<PlayerNormal>(m_player, m_childState);
    }

    // 必殺技
    // 大抵のモーションをキャンセルできる
    if (p->IsInputSpecialAttack())
    {
        m_childState = std::make_shared<PlayerSpecialAttack>(m_player);
    }

    // 状態をUpdate
    UpdateChildState();

    // 通常のカメラ回転
    p->CameraMove();

    // ロックオンの特殊処理
    CameraMove();

    SetTargetPos();

    // ロックオンボタンを離したら
    if (!input.IsPressed("LockOn") && p->CanLockOn())
    {
        p->ReleaseLockOn();

        // 次のUpdateでNormal状態へ移行
    }

    return shared_from_this();
}

void PlayerLockOn::SetTargetPos()
{
    auto p = m_player.lock();

    // 脳天から線を出す
    auto pTargetStart = p->GetPos() + kLockOnLineStartOffset;
    auto eTargetStart = p->m_lockOnActor.lock()->GetPos() + kLockOnLineStartOffset;
    auto playerToEnemy = eTargetStart - pTargetStart;
    auto pToELength = playerToEnemy.Magnitude();
    auto pToEN = playerToEnemy.GetNormalize();

    // targetPosを更新
    p->m_targetPos = pTargetStart + pToEN * (pToELength * m_targetPosLerpParam.Value());
}

void PlayerLockOn::CameraMove()
{
    // ここでやってること
    // 1.プレイヤーがスクリーンX方向に出ようとしたら収めるように回転する
    // 2.プレイヤーがカメラの後ろに回りそうならカメラの距離を離す
    // 3.プレイヤーや敵がスクリーンのY方向に出そうなら注視点を寄せて画面に収める

    auto& input = Input::GetInstance();
    auto p = m_player.lock();
    auto camera = p->m_camera.lock();

    // プレイヤーが画面外に出たら
    // その向きにカメラ回転
    auto playerScreenPos = ConvWorldPosToScreenPos(p->GetPos() + kPlayerMiddlePointOffset);
    auto targetScreenPos = ConvWorldPosToScreenPos(camera->GetTargetPos());

    const float nearFarLength = camera->GetCameraNearFarLength();
    const float cameraRotatableOffset = kCameraAutoRotateDepthOffset / nearFarLength;

    float rightOutOfX = std::min(playerScreenPos.x - Game::kScreenWidth * (1.0f - kScreenFitThresholdX), kCameraRotSpeedMax);
    float leftOutOfX = std::min(Game::kScreenWidth * kScreenFitThresholdX - playerScreenPos.x, kCameraRotSpeedMax);

    // 条件複雑だな
    if (rightOutOfX > 0 && playerScreenPos.z > 0 && playerScreenPos.z < targetScreenPos.z - cameraRotatableOffset)
    {
        p->m_camera.lock()->RotateCameraUpVecY(-rightOutOfX * kCameraRotSpeed);
    }
    if (leftOutOfX > 0 && playerScreenPos.z > 0 && playerScreenPos.z < targetScreenPos.z - cameraRotatableOffset)
    {
        p->m_camera.lock()->RotateCameraUpVecY(leftOutOfX * kCameraRotSpeed);
    }

    // カメラの回転円の外側にプレイヤーや敵が出そうなら
    // カメラの回転半径を伸ばす
    auto pToE = p->m_lockOnActor.lock()->GetPos() - p->GetPos();
    auto pToELength = pToE.Magnitude();
    auto pToEN = pToE.GetNormalize();
    auto middlePos =  p->GetPos() +  pToEN * (pToELength * kMiddleTargetPos);
    auto targetDist = (middlePos - p->GetPos()).SqrMagnitude();
    // カメラがプレイヤーを収めるために必要なカメラ距離の補正
    auto cameraGetAwayThireshold = camera->GetTargetDistance() - kPlayerIntoCameraXZOffset;

    if (targetDist > cameraGetAwayThireshold * cameraGetAwayThireshold)
    {
        camera->SetTargetDistance(camera->GetTargetDistance() + kChangeDistanceSpeed);
    }
    else if (camera->GetTargetDistance() > kDefaultCameraDistance)
    {
        camera->SetTargetDistance(camera->GetTargetDistance() + -kChangeDistanceSpeed);
    }

    auto playerFootScrrenPos = ConvWorldPosToScreenPos(p->GetPos());
    auto enemyScreenPos = ConvWorldPosToScreenPos(p->m_lockOnActor.lock()->GetPos());

    // プレイヤーが手前の場合
    if (playerFootScrrenPos.z < enemyScreenPos.z)
    {
        if (playerFootScrrenPos.y > Game::kScreenHeight * kGetAwayTargetPosScreenHeightThreshold)
        {
            m_targetPosLerpParam -= kMoveTargetPosSpeed;
        }
        else if (playerFootScrrenPos.y < Game::kScreenHeight * kStopMoveTargetPosScreenHeightThreshold)
        {
            m_targetPosLerpParam += kMoveTargetPosSpeed;
            m_targetPosLerpParam = std::min(m_targetPosLerpParam.Value(), kMiddleTargetPos);
        }
    }
    else
    {
        if (enemyScreenPos.y > Game::kScreenHeight * kGetAwayTargetPosScreenHeightThreshold)
        {
            m_targetPosLerpParam += kMoveTargetPosSpeed;
        }
        else if (enemyScreenPos.y < Game::kScreenHeight * kStopMoveTargetPosScreenHeightThreshold)
        {
            m_targetPosLerpParam -= kMoveTargetPosSpeed;
            m_targetPosLerpParam = std::max(m_targetPosLerpParam.Value(), kMiddleTargetPos);
        }
    }
}
