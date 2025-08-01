#include "PlayerHPBar.h"
#include "Image.h"
#include <DxLib.h>
#include <cmath>
#include "../Actor/Player/Player.h"

namespace
{
	const std::string kBaseImageName = "BaseImage.png";
	const std::string kFillImageName = "FillImage.png";
	constexpr float kLerpSpeed = 0.1f;
	const Vector3 kInitPos = { 50, 50, 0 };
}

PlayerHPBar::PlayerHPBar()
{
}

void PlayerHPBar::Init(std::weak_ptr<Player> player)
{
	m_pos = kInitPos;

	m_player = player;

	m_baseImage = std::make_shared<Image>();
	m_baseImage->Init(kBaseImageName);
	m_fillImage = std::make_shared<Image>();
	m_fillImage->Init(kFillImageName);

	m_fillImageSize = m_fillImage->GetImageSize();
}

void PlayerHPBar::Update()
{
	// やりたいこと
	// 毎フレームプレイヤーのHPを見て、変化があったらアニメーションとともにゲージを変化させる
	// 今のHPと最大HPの割合を見て、ゲージの色を変える

	m_fillRatio = std::lerp(m_fillRatio.Value(), m_targetFillRatio.Value(), kLerpSpeed);


	float nowHpRatio = m_player.lock()->GetHpRatio();
	if (m_targetFillRatio.Value() != nowHpRatio)
	{
		m_targetFillRatio = nowHpRatio;
		// 差分で回復かダメージか判断
		// 演出の状態を変える
	}
}

void PlayerHPBar::Draw() const
{
	Vector2 drawPos = { m_pos.x, m_pos.y };
	m_baseImage->Draw(drawPos + m_fillImageSize * 0.5f);
	m_fillImage->RectDraw(drawPos, Vector2::Zero(), { m_fillImageSize.x * m_fillRatio.Value(), m_fillImageSize.y});
}
