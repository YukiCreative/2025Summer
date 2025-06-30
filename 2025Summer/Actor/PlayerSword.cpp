#include "PlayerSword.h"
#include "CapsuleCollider.h"
#include "Collidable.h"

namespace
{
	constexpr float kSwordLength = 300.0f;
	constexpr float kSowrdRadius = 50.0f;
}

PlayerSword::PlayerSword() :
	Actor(false)
{
}

void PlayerSword::Init()
{
	auto cCol = std::make_shared<CapsuleCollider>();
	cCol->Init(m_pos, m_pos + Vector3::Up() * kSwordLength, 10, true, false, kSowrdRadius);
}

void PlayerSword::Update()
{
	// プレイヤーの手の位置を取得する

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
