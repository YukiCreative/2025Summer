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
	// ���̏�ԑJ�ڎ��ɁA�󒆂̋�C��R�����炷
	m_player.lock()->GetRigid().SetAirDrag(kHoveringDrag);
}

PlayerHovering::~PlayerHovering()
{
	m_player.lock()->GetRigid().SetAirDrag(m_player.lock()->DefaultAirDrag());
}

std::shared_ptr<PlayerState> PlayerHovering::Update()
{
	// ��莞�ԑ҂�

	if (m_frame > kHoveringTime)
	{
		return std::make_shared<PlayerFall>(m_player);
	}

	++m_frame;

	return shared_from_this();
}
