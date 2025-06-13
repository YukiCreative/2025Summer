#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"
#include "Collidable.h"
#include "Rigid.h"
#include "SphereCollider.h"

Player::Player()
{
}

void Player::Init()
{
	m_model = std::make_shared<AnimationModel>();
	m_model->Init("Data/Model/Player.mv1", 3);

	auto col = std::make_shared<SphereCollider>();
	col->Init(m_pos, 1, false, false, 1);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init();

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init();
}

void Player::Update()
{
	// “ü—Í‚ÅˆÚ“®
	const Vector2 inputAxis = Input::GetInstance().GetLeftInputAxis();
	m_collidable->AddVel(Vector3{ inputAxis .x, 0,inputAxis.y });
}

void Player::Draw() const
{
	m_model->Draw();
}

void Player::CommitMove()
{
	// ‘¬“xŒ¸Š‚³‚¹‚ÄŽæ“¾
	m_pos += m_collidable->UpdateRigid();
}
