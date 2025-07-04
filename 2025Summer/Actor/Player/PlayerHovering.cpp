#include "PlayerHovering.h"
#include "Player.h"
#include "Rigid.h"
#include "PlayerFall.h"
#include "AnimationModel.h"

namespace
{
	constexpr float kHoveringDrag = 0.0f;
	constexpr int   kHoveringTime = 20;
	const std::string kAnimName = "Armature|FrontFlip";
}

PlayerHovering::PlayerHovering(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
	// この状態遷移時に、空中の空気抵抗を減らす
	m_player.lock()->GetRigid().SetAirDrag(kHoveringDrag);
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);

	m_player.lock()->GetRigid().AddVel({0,5,0});
}

PlayerHovering::~PlayerHovering()
{
	m_player.lock()->GetRigid().SetAirDrag(m_player.lock()->DefaultAirDrag());
}

std::shared_ptr<PlayerState> PlayerHovering::Update()
{
	// 一定時間待つ

	MoveCameraTargetIgnoreY(0.01f);

	if (m_frame > kHoveringTime)
	{
		return std::make_shared<PlayerFall>(m_player);
	}

	++m_frame;

	return shared_from_this();
}
