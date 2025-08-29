#include "PlayerLockOnUI.h"
#include "../Actor/Player/Player.h"
#include "Image.h"
#include <cmath>
#include "../Actor/Enemy/Enemy.h"

namespace
{
	const std::string kOutsideImageName = "PlayerLockOnCursor_Outside.png";
	const std::string kHitPointImageName = "Data/Image/PlayerLockOnCursor_HitPoint.png";
	const std::string kStunPointImageName = "Data/Image/PlayerLockOnCursor_StunPoint.png";
	constexpr float kStateSpeed = 0.15f;
	constexpr float kTransparentSpeed = 0.05f;
	constexpr float kStartExRate = 5.0f;
	constexpr float kEndExRate = 1.0f;
	constexpr float kLerpSpeed = 0.2f;
}

PlayerLockOnUI::PlayerLockOnUI() :
	UIBase(ToString(PlayerLockOnUI)),
	m_stateParam(0),
	m_state(&PlayerLockOnUI::NoLockOn),
	m_hitPointImgHandle(-1),
	m_stunPointImgHandle(-1),
	m_hitPointGraphRatio(1.0f),
	m_stunPointGraphRatio(1.0f)
{
}

void PlayerLockOnUI::Init(std::weak_ptr<Player> player)
{
	m_player = player;

	m_image = std::make_shared<Image>();
	m_image->Init(kOutsideImageName);

	m_image->SetImageBlendMode(DX_BLENDMODE_ALPHA, 0);

	m_stateParam.SetMax();
	m_image->SetExRate(kStartExRate);

	m_hitPointImgHandle = LoadGraph(kHitPointImageName.c_str());
	m_stunPointImgHandle = LoadGraph(kStunPointImageName.c_str());
}

void PlayerLockOnUI::Update()
{
	// 演出ステート
	(this->*m_state)();

	if (!m_player.lock()->IsLockOn()) return;
	std::weak_ptr<Enemy> lockOnEnemy = m_player.lock()->GetLockOnEnemy();

	m_hitPointGraphRatio = std::lerp(m_hitPointGraphRatio, lockOnEnemy.lock()->GetHpRatio(), kLerpSpeed);
	m_stunPointGraphRatio = std::lerp(m_stunPointGraphRatio, lockOnEnemy.lock()->GetStunRatio(), kLerpSpeed);
}

void PlayerLockOnUI::Draw() const
{
	if (!m_player.lock()->IsLockOn()) return;
	std::weak_ptr<Enemy> lockOnEnemy =  m_player.lock()->GetLockOnEnemy();

	DrawCircleGauge(m_pos.x, m_pos.y, m_hitPointGraphRatio * 100.0f, m_hitPointImgHandle, 0.0, kEndExRate);
	DrawCircleGauge(m_pos.x, m_pos.y, m_stunPointGraphRatio * 100.0f, m_stunPointImgHandle, 0.0f, kEndExRate);

	m_image->Draw(m_pos);
}

void PlayerLockOnUI::StartLockOn()
{
	MoveToLockOnTarget();

	m_stateParam -= kStateSpeed;

	// 縮小しながら透過度を下げる
	m_image->SetExRate(std::lerp(kEndExRate, kStartExRate, m_stateParam.Value()));
	m_image->SetImageBlendParam(255 - m_stateParam.Value() * 255);

	// 終わったらDuringへ
	if (m_stateParam.IsMin())
	{
		m_image->SetExRate(kEndExRate);
		m_state = &PlayerLockOnUI::DuringLockOn;
	}

	// 演出中再度ロックオンが外れたらEndへ
	if (!m_player.lock()->IsLockOn())
	{
		m_state = &PlayerLockOnUI::EndLockOn;
	}
}

void PlayerLockOnUI::EndLockOn()
{
	// 拡大しながら透明になっていく
	m_stateParam += kStateSpeed;

	m_image->SetExRate(std::lerp(kEndExRate, kStartExRate, m_stateParam.Value()));
	m_image->SetImageBlendParam(255 - m_stateParam.Value() * 255);

	// 終わったらNoへ
	if (m_stateParam.IsMax())
	{
		m_image->SetExRate(kStartExRate);
		m_state = &PlayerLockOnUI::NoLockOn;
	}

	// 演出中再度ロックオンしたらStartへ
	if (m_player.lock()->IsLockOn())
	{
		m_state = &PlayerLockOnUI::StartLockOn;
	}
}

void PlayerLockOnUI::DuringLockOn()
{
	// 演出なし

	MoveToLockOnTarget();

	// Endへの遷移条件
	if (!m_player.lock()->IsLockOn())
	{
		m_state = &PlayerLockOnUI::EndLockOn;
	}
}

void PlayerLockOnUI::NoLockOn()
{
	// 演出なし

	// Startへの遷移条件
	if (m_player.lock()->IsLockOn())
	{
		m_state = &PlayerLockOnUI::StartLockOn;
	}
}

void PlayerLockOnUI::MoveToLockOnTarget()
{
	if (!m_player.lock()->IsLockOn()) return;

	// ロックオンされている対象のスクリーン上の位置に移動
	auto lockOnActorScreenPos = m_player.lock()->GetLockOnActorScreenPos();

	m_pos.x = lockOnActorScreenPos.x;
	m_pos.y = lockOnActorScreenPos.y;
}