#include "PlayerHovering.h"
#include "Player.h"
#include "Rigid.h"
#include "PlayerFall.h"

namespace
{
	constexpr float kHoveringDrag = 0.0f;
	constexpr int   kHoveringTime = 20;
}

PlayerHovering::PlayerHovering(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
	// ‚±‚Ìó‘Ô‘JˆÚŽž‚ÉA‹ó’†‚Ì‹ó‹C’ïR‚ðŒ¸‚ç‚·
	m_player.lock()->GetRigid().SetAirDrag(kHoveringDrag);
}

PlayerHovering::~PlayerHovering()
{
	m_player.lock()->GetRigid().SetAirDrag(m_player.lock()->DefaultAirDrag());
}

std::shared_ptr<PlayerState> PlayerHovering::Update()
{
	// ˆê’èŽžŠÔ‘Ò‚Â

	if (m_frame > kHoveringTime)
	{
		return std::make_shared<PlayerFall>(m_player);
	}

	++m_frame;

	return shared_from_this();
}
