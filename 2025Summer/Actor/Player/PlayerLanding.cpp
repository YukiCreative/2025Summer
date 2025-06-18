#include "PlayerLanding.h"
#include "PlayerIdle.h"
#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"
#include "PlayerMove.h"

namespace
{
	constexpr int kLandFrame = 20;
}

PlayerLanding::PlayerLanding(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_landFrame(0)
{
	// モーションを再生
}

PlayerLanding::~PlayerLanding()
{
}

std::shared_ptr<PlayerState> PlayerLanding::Update()
{
	// モーションが終わるまで継続
	if (m_player.lock()->m_model->IsEnd())
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	// もしくは一定時間経過後、入力があったら遷移
	++m_landFrame;

	if (m_landFrame < kLandFrame) return shared_from_this();

	if (Input::GetInstance().GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
	{
		return std::make_shared<PlayerMove>(m_player);
	}

	return shared_from_this();
}
