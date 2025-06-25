#include "Input.h"
#include "Player.h"
#include "PlayerHovering.h"
#include "PlayerJump.h"
#include "PlayerLanding.h"
#include "Rigid.h"

namespace
{
	const Vector3 kJumpForce = {0, 40, 0};
	constexpr float kJumpTopVel = 5.0f;
	constexpr float kJumpingMoveSpeed = 0.0005f;
}

PlayerJump::PlayerJump(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// 状態の開始処理
	auto& rigid = m_player.lock()->GetRigid();

	rigid.ChangeStateAir();
	auto tempVel = rigid.GetVel();
	tempVel.y = kJumpForce.y;
	rigid.SetVel(tempVel);
}

PlayerJump::~PlayerJump()
{
}

std::shared_ptr<PlayerState> PlayerJump::Update()
{
	m_player.lock()->Move(kJumpingMoveSpeed);

	MoveCameraTargetIgnoreY(0.01f);

	// ジャンプ最高点近くになったら、ちょっと滞空したい
	if (m_player.lock()->GetRigid().GetVel().y < kJumpTopVel)
	{
		return std::make_shared<PlayerHovering>(m_player);
	}

	return shared_from_this();
}