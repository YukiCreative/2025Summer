#include "Input.h"
#include "Player.h"
#include "PlayerDash.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "PlayerJump.h"
#include "AnimationModel.h"
#include "PlayerSlashDown.h"
#include "Collidable.h"
#include <DxLib.h>

namespace
{
	constexpr float kRunSpeed = 0.0018f;
	constexpr int   kDashFrame = 120;

	const std::string kMoveAnimName = "Armature|RunDash";
}

PlayerMove::PlayerMove(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_moveFrame(0)
{
	// アニメーションを遷移
	m_player.lock()->m_model->ChangeAnimation(kMoveAnimName);
}

PlayerMove::~PlayerMove()
{
}

std::shared_ptr<PlayerState> PlayerMove::Update()
{
	Input& input = Input::GetInstance();
	auto p = m_player.lock();

	// 移動
	p->Move(kRunSpeed);

	// 入力が切れたら待機状態へ
	if (input.GetLeftInputAxis().SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	if (input.IsTrigger("Attack"))
	{
		p->GetCollidable().AddVel(VTransformSR({0,0,-30}, p->GetModelMatrix()));
		return std::make_shared<PlayerSlashDown>(m_player);
	}

	++m_moveFrame;

	return shared_from_this();
}
