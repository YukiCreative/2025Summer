#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"

Player::Player()
{
}

void Player::Init()
{
	m_model = std::make_shared<AnimationModel>();
	m_model->Init("Data/Model/Player.mv1", 3);


}

void Player::Update()
{
	// ���͂ňړ�

}

void Player::Draw() const
{
	m_model->Draw();
}

void Player::CommitMove()
{
}
