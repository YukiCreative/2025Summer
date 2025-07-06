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

    constexpr float kRotateSpeed = 0.2f;
}

PlayerLockOn::PlayerLockOn(std::weak_ptr<Player> parent) :
    PlayerState(parent)
{
    auto p = m_player.lock();

    // �������
    m_childState = std::make_shared<PlayerLockOnIdle>(m_player);

    // �J����
    p->m_camera.lock()->ChangeStateDP();

    // �U������������Ă���
    p->DisableSwordCol();
    p->DisableSword();

    // �R�}���h���X�g��������
    p->m_inputList.clear();
}

PlayerLockOn::~PlayerLockOn()
{
}

std::shared_ptr<PlayerState> PlayerLockOn::Update()
{
    auto& input = Input::GetInstance();
    auto p = m_player.lock();

    // ���b�N�I�����O�ꂽ��A�ʏ��Ԃ�
    if (m_player.lock()->m_lockOnActor.expired())
    {
        return std::make_shared<PlayerNormal>(m_player);
    }

    CameraMove();

    // �v���C���[��G�����ɉ�]
    auto lockOnPosXZ = p->m_lockOnActor.lock()->GetPos().XZ();
    auto posXZ = p->GetPos().XZ();

    auto playerToLockOnXZ = (lockOnPosXZ - posXZ).GetNormalize();

    auto playerDir = p->m_model->GetDirection();

    auto dot = playerToLockOnXZ.Dot(playerDir);

    float rot = playerDir.Cross(playerToLockOnXZ).y * kRotateSpeed;

    // ���傤�ǐ^���΂Ɍ����Ă����ꍇ�̏���
    if (dot < -0.9999f && rot < 0.0001f)
    {
        rot += 0.1f;
    }

    p->m_model->RotateUpVecY(rot);

    Vector3 inputAxis = Vector3{ input.GetLeftInputAxis().x, 0, input.GetLeftInputAxis().y };
    inputAxis.z *= -1;
    Vector3 cameraRotatedAxis = p->m_camera.lock()->RotateVecToCameraDirXZ(inputAxis, Vector3::Foward());

    // ��Ԃ�Update
    m_childState = m_childState->Update();

    // �����Ƀ��b�N�I���֘A�̏���������
    // ���b�N�I���{�^������������߂��̓G�����b�N�I��
    if (Input::GetInstance().IsTrigger("LockOn"))
    {
        ReleaseLockOn();

        // ����Update��Normal��Ԃֈڍs
    }

    return shared_from_this();
}

void PlayerLockOn::ReleaseLockOn()
{
    // ����
    m_player.lock()->m_lockOnActor.reset();

    // ���������J���������ɖ߂�
    m_player.lock()->m_camera.lock()->ChangeStateDD(m_player.lock()->GetPos() + kLockOnLineStartOffset);
}

void PlayerLockOn::CameraMove()
{
    // �v���C���[�ƃ��b�N�I���Ώۂ���ʊO�ɏo�Ȃ��悤�ɂ�����
    auto p = m_player.lock();
    auto cam = p->m_camera.lock();

    // �J�����̈ʒu�����߂Ă��܂�

    // ��Ƀ��b�N�I���Ώہ��v���C���[���J�����@�ɂ���

    const Vector3 playerPos = p->GetPos();

    const Vector3 playerToLock = p->m_lockOnActor.lock()->GetPos().XZ() - playerPos.XZ();
    auto mat = MGetRotVec2(Vector3::Foward(), playerToLock);

    // �v���C���[����ʉE����������
    if (ConvWorldPosToScreenPos(playerPos).x < Game::kScreenHalfWidth)
    {
        // �����Ƀv���C���[���J�����͉E����
        cam->SetLerpPos(playerPos + VTransformSR(kLockOnCameraPosOffsetRight, mat));
    }
    else
    {
        // �t���܂��R��
        cam->SetLerpPos(playerPos + VTransformSR(kLockOnCameraPosOffsetLeft, mat));
    }

    // targetPos���X�V
    p->m_targetPos = (playerPos + kLockOnLineStartOffset + p->m_lockOnActor.lock()->GetPos()) * 0.5f;
}
