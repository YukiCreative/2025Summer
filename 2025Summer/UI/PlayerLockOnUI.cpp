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
	// ���o�X�e�[�g
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

	// �k�����Ȃ��瓧�ߓx��������
	m_image->SetExRate(std::lerp(kEndExRate, kStartExRate, m_stateParam.Value()));
	m_image->SetImageBlendParam(255 - m_stateParam.Value() * 255);

	// �I�������During��
	if (m_stateParam.IsMin())
	{
		m_image->SetExRate(kEndExRate);
		m_state = &PlayerLockOnUI::DuringLockOn;
	}

	// ���o���ēx���b�N�I�����O�ꂽ��End��
	if (!m_player.lock()->IsLockOn())
	{
		m_state = &PlayerLockOnUI::EndLockOn;
	}
}

void PlayerLockOnUI::EndLockOn()
{
	// �g�債�Ȃ��瓧���ɂȂ��Ă���
	m_stateParam += kStateSpeed;

	m_image->SetExRate(std::lerp(kEndExRate, kStartExRate, m_stateParam.Value()));
	m_image->SetImageBlendParam(255 - m_stateParam.Value() * 255);

	// �I�������No��
	if (m_stateParam.IsMax())
	{
		m_image->SetExRate(kStartExRate);
		m_state = &PlayerLockOnUI::NoLockOn;
	}

	// ���o���ēx���b�N�I��������Start��
	if (m_player.lock()->IsLockOn())
	{
		m_state = &PlayerLockOnUI::StartLockOn;
	}
}

void PlayerLockOnUI::DuringLockOn()
{
	// ���o�Ȃ�

	MoveToLockOnTarget();

	// End�ւ̑J�ڏ���
	if (!m_player.lock()->IsLockOn())
	{
		m_state = &PlayerLockOnUI::EndLockOn;
	}
}

void PlayerLockOnUI::NoLockOn()
{
	// ���o�Ȃ�

	// Start�ւ̑J�ڏ���
	if (m_player.lock()->IsLockOn())
	{
		m_state = &PlayerLockOnUI::StartLockOn;
	}
}

void PlayerLockOnUI::MoveToLockOnTarget()
{
	if (!m_player.lock()->IsLockOn()) return;

	// ���b�N�I������Ă���Ώۂ̃X�N���[����̈ʒu�Ɉړ�
	auto lockOnActorScreenPos = m_player.lock()->GetLockOnActorScreenPos();

	// Z�͕ω����Ăق����Ȃ�
	m_pos.x = lockOnActorScreenPos.x;
	m_pos.y = lockOnActorScreenPos.y;
}