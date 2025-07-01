#include "PlayerSword.h"
#include "CapsuleCollider.h"
#include "Collidable.h"
#include "Player.h"

namespace
{
	constexpr float kSwordLength = 300.0f;
	constexpr float kSowrdRadius = 50.0f;

	const std::string kModelName = "Data/Model/greatsword.mv1";
}

PlayerSword::PlayerSword() :
	Actor(false)
{
}

void PlayerSword::Init(std::weak_ptr<Player> player)
{
	auto cCol = std::make_shared<CapsuleCollider>();
	cCol->Init(m_pos, m_pos + Vector3::Up() * kSwordLength, 10, true, false, kSowrdRadius);

	m_model = std::make_shared<Model>();
	m_model->Init(kModelName);

	m_player = player;
}

void PlayerSword::Update()
{
	// プレイヤーの手の位置を取得する
	m_pos = m_player.lock()->GetRightHandVec();
}

void PlayerSword::Draw() const
{
	m_model->Draw();
}

void PlayerSword::OnCollision(const std::shared_ptr<Actor> other)
{
}

void PlayerSword::CommitMove()
{
	auto vel = m_collidable->UpdateRigid();
	m_pos += vel;
	
	m_collidable->SetPos(m_pos);
}
