#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerJump.h"
#include "PlayerMove.h"
#include "AnimationModel.h"
#include "PlayerLockOnIdle.h"

namespace
{
	const std::string kAnimName = "Armature|Idle";
}

PlayerIdle::PlayerIdle(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName);
}

PlayerIdle::~PlayerIdle()
{
}

std::shared_ptr<PlayerState> PlayerIdle::Update()
{
	// âΩÇ‡ÇµÇ»Ç¢
	Input& input = Input::GetInstance();

	MoveCameraTarget();

	// ì¸óÕÇ™Ç†Ç¡ÇΩÇÁ
	if (input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
	{
		return std::make_shared<PlayerMove>(m_player);
	}
	if (input.IsTrigger("Jump"))
	{
		return std::make_shared<PlayerJump>(m_player);
	}
	if (!m_player.lock()->m_lockOnActor.expired())
	{
		// ÉçÉbÉNÉIÉì
		return std::make_shared<PlayerLockOnIdle>(m_player);
	}

	return shared_from_this();
}
