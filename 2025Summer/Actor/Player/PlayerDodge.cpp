#include "PlayerDodge.h"

PlayerDodge::PlayerDodge(std::weak_ptr<Player> parent) :
	PlayerState(parent),
	m_frame(0)
{
}

PlayerDodge::~PlayerDodge()
{
}

std::shared_ptr<PlayerState> PlayerDodge::Update()
{
	// モーションを流す
	// 特定フレームで無敵
	// 特定フレームの間に攻撃を受けるとジャスト回避

	++m_frame;

	return shared_from_this();
}
