#include "PlayerNormal.h"
#include "Player.h"
#include "PlayerLockOn.h"
#include "PlayerIdle.h"
#include "Input.h"
#include "ActorController.h"
#include <DxLib.h>
#include "Game.h"
#include "Camera.h"
#include "PlayerAttackState.h"
#include "PlayerSpecialAttack.h"

PlayerNormal::PlayerNormal(std::weak_ptr<Player> parent, std::shared_ptr<PlayerState> initState) :
	PlayerIntermediateState(parent)
{
	// ���b�N�I����Ԃ��܂����������X�e�[�g�Ȃ瑱�s
	if (initState->CanCrossState())
	{
		m_childState = initState;
	}
	else
	{
		m_childState = std::make_shared<PlayerIdle>(parent);
	}

	m_player.lock()->m_camera.lock()->SetCameraDistanceDefault();

	// �U������������Ă���
	m_player.lock()->DisableSwordCol();
	m_player.lock()->DisableSword();
}

PlayerNormal::~PlayerNormal()
{
}

std::shared_ptr<PlayerIntermediateState> PlayerNormal::Update()
{
	auto p = m_player.lock();
	auto& input = Input::GetInstance();

	// ���b�N�I�����ꂽ��
	if (!p->m_lockOnActor.expired())
	{
		return std::make_shared<PlayerLockOn>(m_player, m_childState);
	}

	// �K�E�Z
	// ���̃��[�V�������L�����Z���ł���
	if (p->IsInputSpecialAttack())
	{
		m_childState = std::make_shared<PlayerSpecialAttack>(m_player);
	}

	// �ʏ�̃J������]
	p->CameraMove();

	UpdateChildState();

	MoveCameraTarget();

	// ���b�N�I���{�^������������߂��̓G�����b�N�I��
	if (Input::GetInstance().IsTrigger("LockOn") && p->CanLockOn())
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

void PlayerNormal::MoveCameraTarget()
{
	m_player.lock()->m_targetPos = m_player.lock()->GetPos() + kCameraTargetOffset;
}