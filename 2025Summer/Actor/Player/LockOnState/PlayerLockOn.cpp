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
    // �v���C���[���J�����Ɏ��܂邽�߂�XZ���ʏ�̋���
    // �J�����͂��̋�������]���a��L�΂�
    constexpr float kPlayerIntoCameraXZOffset = 300.0f;
    constexpr float kMoveTargetPosSpeed = 0.01f;

    constexpr float kDefaultCameraDistance = 400.0f;

    // �X�N���[���������̃v���C���[�����߂悤�Ƃ���͈�
    constexpr float kScreenFitThresholdX = 0.2f;
    constexpr float kScreenFitThresholdY = 0.1f;

    constexpr float kCameraAutoRotateDepthOffset = 200.0f;

    constexpr float kGetAwayTargetPosScreenHeightThreshold = 0.9f;
    constexpr float kStopMoveTargetPosScreenHeightThreshold = 0.8f;

    // �v���C���[�� 0�`1 �G�l�~�[��
    constexpr float kMiddleTargetPos = 0.6f;
}

PlayerLockOn::PlayerLockOn(std::weak_ptr<Player> parent, std::shared_ptr<PlayerState> initState) :
    PlayerIntermediateState(parent),
    m_targetPosLerpParam(0.5f)
{
    auto p = m_player.lock();

    // ���b�N�I����Ԃ��܂����������X�e�[�g�Ȃ瑱�s
    if (initState->CanCrossState())
    {
        m_childState = initState;
    }
    else
    {
        m_childState = std::make_shared<PlayerLockOnIdle>(parent);
    }

    // �U������������Ă���
    p->DisableSwordCol();
    p->DisableSword();

    // �R�}���h���X�g��������
    p->m_inputList.clear();

    // �J�������}�Ɉړ����Ȃ��悤�ɂ���
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

    // ���b�N�I�����O�ꂽ��A�ʏ��Ԃ�
    if (m_player.lock()->m_lockOnActor.expired())
    {
        return std::make_shared<PlayerNormal>(m_player, m_childState);
    }

    // �K�E�Z
    // ���̃��[�V�������L�����Z���ł���
    if (p->IsInputSpecialAttack())
    {
        m_childState = std::make_shared<PlayerSpecialAttack>(m_player);
    }

    // ��Ԃ�Update
    UpdateChildState();

    // �ʏ�̃J������]
    p->CameraMove();

    // ���b�N�I���̓��ꏈ��
    CameraMove();

    SetTargetPos();

    // ���b�N�I���{�^���𗣂�����
    if (!input.IsPressed("LockOn") && p->CanLockOn())
    {
        p->ReleaseLockOn();

        // ����Update��Normal��Ԃֈڍs
    }

    return shared_from_this();
}

void PlayerLockOn::SetTargetPos()
{
    auto p = m_player.lock();

    // �]�V��������o��
    auto pTargetStart = p->GetPos() + kLockOnLineStartOffset;
    auto eTargetStart = p->m_lockOnActor.lock()->GetPos() + kLockOnLineStartOffset;
    auto playerToEnemy = eTargetStart - pTargetStart;
    auto pToELength = playerToEnemy.Magnitude();
    auto pToEN = playerToEnemy.GetNormalize();

    // targetPos���X�V
    p->m_targetPos = pTargetStart + pToEN * (pToELength * m_targetPosLerpParam.Value());
}

void PlayerLockOn::CameraMove()
{
    // �����ł���Ă邱��
    // 1.�v���C���[���X�N���[��X�����ɏo�悤�Ƃ�������߂�悤�ɉ�]����
    // 2.�v���C���[���J�����̌��ɉ�肻���Ȃ�J�����̋����𗣂�
    // 3.�v���C���[��G���X�N���[����Y�����ɏo�����Ȃ璍���_���񂹂ĉ�ʂɎ��߂�

    auto& input = Input::GetInstance();
    auto p = m_player.lock();
    auto camera = p->m_camera.lock();

    // �v���C���[����ʊO�ɏo����
    // ���̌����ɃJ������]
    auto playerScreenPos = ConvWorldPosToScreenPos(p->GetPos() + kPlayerMiddlePointOffset);
    auto targetScreenPos = ConvWorldPosToScreenPos(camera->GetTargetPos());

    const float nearFarLength = camera->GetCameraNearFarLength();
    const float cameraRotatableOffset = kCameraAutoRotateDepthOffset / nearFarLength;

    float rightOutOfX = std::min(playerScreenPos.x - Game::kScreenWidth * (1.0f - kScreenFitThresholdX), kCameraRotSpeedMax);
    float leftOutOfX = std::min(Game::kScreenWidth * kScreenFitThresholdX - playerScreenPos.x, kCameraRotSpeedMax);

    // �������G����
    if (rightOutOfX > 0 && playerScreenPos.z > 0 && playerScreenPos.z < targetScreenPos.z - cameraRotatableOffset)
    {
        p->m_camera.lock()->RotateCameraUpVecY(-rightOutOfX * kCameraRotSpeed);
    }
    if (leftOutOfX > 0 && playerScreenPos.z > 0 && playerScreenPos.z < targetScreenPos.z - cameraRotatableOffset)
    {
        p->m_camera.lock()->RotateCameraUpVecY(leftOutOfX * kCameraRotSpeed);
    }

    // �J�����̉�]�~�̊O���Ƀv���C���[��G���o�����Ȃ�
    // �J�����̉�]���a��L�΂�
    auto pToE = p->m_lockOnActor.lock()->GetPos() - p->GetPos();
    auto pToELength = pToE.Magnitude();
    auto pToEN = pToE.GetNormalize();
    auto middlePos =  p->GetPos() +  pToEN * (pToELength * kMiddleTargetPos);
    auto targetDist = (middlePos - p->GetPos()).SqrMagnitude();
    // �J�������v���C���[�����߂邽�߂ɕK�v�ȃJ���������̕␳
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

    // �v���C���[����O�̏ꍇ
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
