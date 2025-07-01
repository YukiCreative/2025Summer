#include "Input.h"
#include "Player.h"
#include "PlayerDash.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "PlayerJump.h"
#include "AnimationModel.h"

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

	// 移動
	m_player.lock()->Move(kRunSpeed);

	MoveCameraTarget();

	// 入力が切れたら待機状態へ
	if (input.GetLeftInputAxis().SqrMagnitude() < kMoveThreshold)
	{
		return std::make_shared<PlayerIdle>(m_player);
	}
	// 動き続けて一定時間経過したらダッシュ
	//if (m_moveFrame > kDashFrame)
	//{
	//	return std::make_shared<PlayerDash>(m_player);
	//}
	//if (input.IsTrigger("Jump"))
	//{
	//	return std::make_shared<PlayerJump>(m_player);
	//}

	++m_moveFrame;

	return shared_from_this();
}
