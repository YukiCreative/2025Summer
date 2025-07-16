#include "PlayerLockOnUI.h"
#include "Player.h"
#include "Image.h"
#include <cmath>

namespace
{
	const std::string kImageName = "LockOnCursor.png";
	constexpr float kStateSpeed = 0.15f;
	constexpr float kTransparentSpeed = 0.05f;
	constexpr float kStartExRate = 5.0f;
	constexpr float kEndExRate = 1.0f;
}

PlayerLockOnUI::PlayerLockOnUI() :
	m_stateParam(0)
{
}

void PlayerLockOnUI::Init(std::weak_ptr<Player> player)
{
	m_player = player;

	m_image = std::make_shared<Image>();
	m_image->Init(kImageName);

	m_image->SetImageBlendMode(DX_BLENDMODE_ALPHA, 0);

	m_stateParam.SetMax();
	m_image->SetExRate(kStartExRate);

	m_state = &PlayerLockOnUI::NoLockOn;
}

void PlayerLockOnUI::Update()
{
	// 演出ステート
	(this->*m_state)();
}

void PlayerLockOnUI::Draw() const
{
	m_image->Draw({ m_pos.x, m_pos.y });
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

	// Zは変化してほしくない
	m_pos.x = lockOnActorScreenPos.x;
	m_pos.y = lockOnActorScreenPos.y;
}