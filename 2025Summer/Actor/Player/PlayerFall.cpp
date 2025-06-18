#include "PlayerFall.h"
#include "Player.h"
#include "PlayerLanding.h"
#include "Rigid.h"
#include "Camera.h"

namespace
{
	constexpr float kFallingMoveSpeed = 0.0005f;
	const Vector3 kAddGravity = { 0,-1.5f,0 };
}

PlayerFall::PlayerFall(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
}

PlayerFall::~PlayerFall()
{
}

std::shared_ptr<PlayerState> PlayerFall::Update()
{
	// ���n���邩�U�����邩�U�����󂯂邩�������ԑJ��

	// �����̋������C�ɓ���Ȃ��̂Œǉ��ŗ��Ƃ�
	m_player.lock()->GetRigid().AddVel(kAddGravity);

	// �ړ�
	m_player.lock()->Move(kFallingMoveSpeed);
	m_player.lock()->CameraMove();

	MoveCameraTargetIgnoreY(0.01f);

	m_player.lock()->m_camera.lock()->SetTargetPos({0,0,0});

	if (m_player.lock()->GetPos().y < 0)
	{
 		return std::make_shared<PlayerLanding>(m_player);
	}

	return shared_from_this();
}
