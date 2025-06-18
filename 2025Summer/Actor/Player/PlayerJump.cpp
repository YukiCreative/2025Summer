#include "PlayerJump.h"
#include "Player.h"
#include "Input.h"
#include "Rigid.h"
#include "PlayerLanding.h"

namespace
{
	const Vector3 kJumpForce = {0, 50, 0};
	constexpr float kJumpTopVel = 0.05f;
}

PlayerJump::PlayerJump(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// 状態の開始処理
	auto& rigid = m_player.lock()->GetRigid();

	rigid.ChangeStateAir();
	rigid.AddVel(kJumpForce);
}

PlayerJump::~PlayerJump()
{
}

std::shared_ptr<PlayerState> PlayerJump::Update()
{
	m_player.lock()->CameraMove();

	// 地面についたら着地モーションへ
	// 当たり判定ができるまでは仮
	if (m_player.lock()->GetPos().y < 0)
	{
		return std::make_shared<PlayerLanding>(m_player);
	}
	// ジャンプ最高点近くになったら、ちょっと滞空したい
	if (m_player.lock()->GetRigid().GetVel().y < kJumpTopVel)
	{
		return std::make_shared<>
	}

	return shared_from_this();
}
