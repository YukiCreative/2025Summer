#include "PlayerTeleportation.h"
#include "Player.h"
#include "../../Physics/Collider/Collider3D.h"
#include "../../Physics/Collider/CapsuleCollider.h"
#include <DxLib.h>
#include "NormalState//PlayerIdle.h"
#include "../../General/Input.h"
#include "NormalState/PlayerMove.h"

namespace
{
	constexpr int kDisappearFrame = 3;
	const std::string kStunAnimName = "Armature|FrontStop";

	constexpr int kTelePortFrame = 5;

	constexpr int kAppearFrame = 10;

	const Vector3 kBackPos = { 0.0f, 0.0f, -300.0f };

	constexpr int kStateWholeFrame = 20;
	constexpr int kCanTransitionFrame = 15;

	constexpr float kTeleDistance = 150.0f;
}

PlayerTeleportation::PlayerTeleportation(std::weak_ptr<Player> player) :
	PlayerState(player),
	m_frame(0)
{
	m_player.lock()->ChangeAnim(kStunAnimName, false);
	m_canCrossState = true;
	m_player.lock()->SetInvincibility(true);
}

PlayerTeleportation::~PlayerTeleportation()
{
	m_player.lock()->SetInvincibility(false);
	m_player.lock()->GetCol().SetIsThrough(false);
}

std::shared_ptr<PlayerState> PlayerTeleportation::Update()
{
	auto player = m_player.lock();
	Input& input = Input::GetInstance();

	if (m_frame == kDisappearFrame)
	{
		player->Disappear();
		player->GetCol().SetIsThrough(true);
	}

	if (m_frame == kTelePortFrame)
	{
		// ƒƒbƒNƒIƒ“‚µ‚Ä‚é‚©‚µ‚Ä‚È‚¢‚©‚Åˆ—‚ð•ª‚¯‚é
		if (player->IsLockOn())
		{
			Vector3 enemyPos = player->m_lockOnActor.lock()->GetPos();
			// “G‚Ì‚»‚Î‚ÉˆÚ“®
			Vector3 eToPN = (player->GetPos() - enemyPos).GetNormalize();
			Vector3 teleportedPos = enemyPos + eToPN * kTeleDistance;

			player->SetPos(teleportedPos);
		}
		else
		{
			// Œã‚ë‚ÉˆÚ“®
			player->SetPos(player->GetPos() + VTransformSR(kBackPos, player->GetModelMatrix()));
		}
	}

	if (m_frame == kAppearFrame)
	{
		player->Apeear();
	}

	if (m_frame > kCanTransitionFrame)
	{
		if (input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
		{
			return std::make_shared<PlayerMove>(m_player);
		}
	}
	if (m_frame > kStateWholeFrame)
	{
		return std::make_shared<PlayerIdle>(m_player);
	}

	++m_frame;

	return shared_from_this();
}
