#include "AnimationModel.h"
#include "Input.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerMiddleDamage.h"
#include "PlayerMove.h"

namespace
{
	const std::string kAnimName = "Armature|MiddleDamage";

	// 移動可能になるフレーム
	constexpr int kEnableMoveFrame = 30;
	constexpr float kMoveSpeed = 0.001f;
}

PlayerMiddleDamage::PlayerMiddleDamage(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
	m_player.lock()->m_model->ChangeAnimation(kAnimName, false);
	// 剣を消す
	m_player.lock()->DisableSword();
}

PlayerMiddleDamage::~PlayerMiddleDamage()
{
}

std::shared_ptr<PlayerState> PlayerMiddleDamage::Update()
{
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	// アニメーションを完遂or途中で入力があったら遷移
	if (p->m_model->IsEnd())
	{
		return std::make_shared<PlayerIdle>(m_player);
	}

	if (m_frame >= kEnableMoveFrame && input.GetLeftInputAxis().SqrMagnitude() > kMoveThreshold)
	{
		return std::make_shared<PlayerMove>(m_player);
	}

	// ちょっとだけ動ける
	p->Move(kMoveSpeed);

	++m_frame;

	return shared_from_this();
}
