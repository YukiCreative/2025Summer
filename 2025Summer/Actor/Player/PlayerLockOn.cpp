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
    // �������
    m_childState = std::make_shared<PlayerLockOnIdle>(m_player);

    // �J����
    m_player.lock()->m_camera.lock()->ChangeStateDP();
}

PlayerLockOn::~PlayerLockOn()
{
}

std::shared_ptr<PlayerState> PlayerLockOn::Update()
{
    // ���b�N�I�����O�ꂽ��A�ʏ��Ԃ�
    if (m_player.lock()->m_lockOnActor.expired())
    {
        return std::make_shared<PlayerNormal>(m_player);
    }

    CameraMove();

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
    m_player.lock()->m_camera.lock()->ChangeStateDD();
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
