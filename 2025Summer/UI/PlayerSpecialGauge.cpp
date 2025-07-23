#include "PlayerSpecialGauge.h"
#include <string>
#include "Image.h"
#include <cmath>
#include "Player.h"

namespace
{
	const std::string kBaseImageName = "SpecialGaugeBase.png";
	const std::string kFillImageName = "SpecialGaugeFill.png";
	constexpr float kLerpSpeed = 0.1f;
	const Vector3 kInitPos = { 50, 110, 0 };
}

PlayerSpecialGauge::PlayerSpecialGauge()
{
}

void PlayerSpecialGauge::Init(std::weak_ptr<Player> player)
{
	m_pos = kInitPos;

	m_player = player;

	m_baseImage = std::make_shared<Image>();
	m_baseImage->Init(kBaseImageName);
	m_fillImage = std::make_shared<Image>();
	m_fillImage->Init(kFillImageName);

	m_fillImageSize = m_fillImage->GetImageSize();
}

void PlayerSpecialGauge::Update()
{
	// ��肽������
	// ���t���[���v���C���[��HP�����āA�ω�����������A�j���[�V�����ƂƂ��ɃQ�[�W��ω�������
	// ����HP�ƍő�HP�̊��������āA�Q�[�W�̐F��ς���

	m_fillRatio = std::lerp(m_fillRatio.Value(), m_targetFillRatio.Value(), kLerpSpeed);


	float nowHpRatio = m_player.lock()->GetSpecialRatio();
	if (m_targetFillRatio.Value() != nowHpRatio)
	{
		m_targetFillRatio = nowHpRatio;
		// �����ŉ񕜂��_���[�W�����f
		// ���o�̏�Ԃ�ς���
	}
}

void PlayerSpecialGauge::Draw() const
{
	Vector2 drawPos = { m_pos.x, m_pos.y };
	m_baseImage->Draw(drawPos + m_fillImageSize * 0.5f);
	m_fillImage->RectDraw(drawPos, Vector2::Zero(), { m_fillImageSize.x * m_fillRatio.Value(), m_fillImageSize.y });
}
