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
	// ��肽������
	// ���t���[���v���C���[��HP�����āA�ω�����������A�j���[�V�����ƂƂ��ɃQ�[�W��ω�������
	// ����HP�ƍő�HP�̊��������āA�Q�[�W�̐F��ς���

}

void PlayerHPBar::Draw() const
{
	m_baseImage->Draw({ m_pos.x, m_pos.y });
	//DrawBox(0,0,0,0,0,0);
}
