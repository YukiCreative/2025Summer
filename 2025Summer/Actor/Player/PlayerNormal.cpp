#include "PlayerNormal.h"
#include "Player.h"
#include "PlayerLockOn.h"
#include "PlayerIdle.h"
#include "Input.h"
#include "ActorController.h"
#include <DxLib.h>
#include "Game.h"

PlayerNormal::PlayerNormal(std::weak_ptr<Player> parent) :
	PlayerState(parent)
{
	// ������Ԃ�ݒ�
	m_childState = std::make_shared<PlayerIdle>(m_player);
}

PlayerNormal::~PlayerNormal()
{
}

std::shared_ptr<PlayerState> PlayerNormal::Update()
{
	// ���b�N�I�����ꂽ��
	if (!m_player.lock()->m_lockOnActor.expired())
	{
		return std::make_shared<PlayerLockOn>(m_player);
	}

	// �ʏ�̃J������]
	m_player.lock()->CameraMove();

	m_childState = m_childState->Update();

	// ���b�N�I���{�^������������߂��̓G�����b�N�I��
	if (Input::GetInstance().IsTrigger("LockOn"))
	{
		LockOn();

		// ����Update��LockOn��Ԃֈڍs
	}

	return shared_from_this();
}

void PlayerNormal::LockOn()
{
	auto p = m_player.lock();
	auto& actors = p->m_cont;

	// ��ʒ����Ɉ�ԋ߂��A���b�N�I���ł���G��T��
	auto lockOnList = actors.lock()->SearchCanLockOnActor();

	// ��ʓ��ɂ��邩
	ActorList_t inScreenActor;
	for (auto& actor : lockOnList)
	{
		if (!CheckCameraViewClip(actor->GetPos()))
		{
			inScreenActor.emplace_back(actor);
		}
	}

	// �����Ȃ�������A�I���
	if (inScreenActor.empty()) return;

	// �Ƃ肠�����擪�̗v�f�����Ă���
	std::weak_ptr<Actor> centerActor = inScreenActor.front();
	const Vector3 kScreenCenterPos = { Game::kScreenHalfWidth, Game::kScreenHalfHeight, 0 };
	// ���S����̋���(�̓��)
	float centerPosLength = (kScreenCenterPos - ConvWorldPosToScreenPos(centerActor.lock()->GetPos())).SqrMagnitude();

	for (auto& actor : inScreenActor)
	{
		auto screenPos = ConvWorldPosToScreenPos(actor->GetPos());

		// ������̕����߂����
		float actorToCenterLength = (kScreenCenterPos - ConvWorldPosToScreenPos(actor->GetPos())).SqrMagnitude();
		if (actorToCenterLength > centerPosLength) continue;

		// �X�V
		centerActor = actor;
		centerPosLength = actorToCenterLength;
	}

	// ��܂���Actor�Ƀ��b�N�I��
	p->m_lockOnActor = centerActor;
}