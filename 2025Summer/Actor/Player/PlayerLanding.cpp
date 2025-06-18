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
	// ���[�V�������Đ�
}

PlayerLanding::~PlayerLanding()
{
}

std::shared_ptr<PlayerState> PlayerLanding::Update()
{
	// ���[�V�������I���܂Ōp��
	if (m_player.lock()->m_model->IsEnd())
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	// �������͈�莞�Ԍo�ߌ�A���͂���������J��
	++m_landFrame;

	if (m_landFrame < kLandFrame) return shared_from_this();

	if (Input::GetInstance().GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
	{
		return std::make_shared<PlayerMove>(m_player);
	}

	return shared_from_this();
}
