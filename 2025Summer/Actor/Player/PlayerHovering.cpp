#include "PlayerHovering.h"
#include "Player.h"
#include "Rigid.h"

namespace
{
	constexpr float kHo
}

PlayerHovering::PlayerHovering(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// ���̏�ԑJ�ڎ��ɁA�󒆂̋�C��R�����炷
	m_player.lock()->GetRigid().SetAirDrag
}

PlayerHovering::~PlayerHovering()
{
}

std::shared_ptr<PlayerState> PlayerHovering::Update()
{


	return shared_from_this();
}
