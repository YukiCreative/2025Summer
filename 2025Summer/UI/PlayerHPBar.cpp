#include "PlayerHPBar.h"
#include "Image.h"
#include <DxLib.h>

PlayerHPBar::PlayerHPBar()
{
}

void PlayerHPBar::Init()
{

}

void PlayerHPBar::Update()
{
	// やりたいこと
	// 毎フレームプレイヤーのHPを見て、変化があったらアニメーションとともにゲージを変化させる
	// 今のHPと最大HPの割合を見て、ゲージの色を変える

}

void PlayerHPBar::Draw() const
{
	m_baseImage->Draw({ m_pos.x, m_pos.y });
	//DrawBox(0,0,0,0,0,0);
}
