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
	// ロックオン状態をまたがせたいステートなら続行
	if (initState->CanCrossState())
	{
		m_childState = initState;
	}
	else
	{
		m_childState = std::make_shared<PlayerIdle>(parent);
	}

	m_player.lock()->m_camera.lock()->SetCameraDistanceDefault();

	// 攻撃判定を消しておく
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

	// ロックオンされたら
	if (!p->m_lockOnActor.expired())
	{
		return std::make_shared<PlayerLockOn>(m_player, m_childState);
	}

	// 必殺技
	// 大抵のモーションをキャンセルできる
	if (p->IsInputSpecialAttack())
	{
		m_childState = std::make_shared<PlayerSpecialAttack>(m_player);
	}

	// 通常のカメラ回転
	p->CameraMove();

	UpdateChildState();

	MoveCameraTarget();

	// ロックオンボタンを押したら近くの敵をロックオン
	if (Input::GetInstance().IsTrigger("LockOn") && p->CanLockOn())
	{
		LockOn();

		// 次のUpdateでLockOn状態へ移行
	}

	return shared_from_this();
}

void PlayerNormal::LockOn()
{
	auto p = m_player.lock();
	auto& actors = p->m_cont;

	// 画面中央に一番近い、ロックオンできる敵を探す
	auto lockOnList = actors.lock()->SearchCanLockOnActor();

	// 画面内にいるか
	ActorList_t inScreenActor;
	for (auto& actor : lockOnList)
	{
		if (!CheckCameraViewClip(actor->GetPos()))
		{
			inScreenActor.emplace_back(actor);
		}
	}

	// 何もなかったら、終わり
	if (inScreenActor.empty()) return;

	// とりあえず先頭の要素を入れておく
	std::weak_ptr<Actor> centerActor = inScreenActor.front();
	const Vector3 kScreenCenterPos = { Game::kScreenHalfWidth, Game::kScreenHalfHeight, 0 };
	// 中心からの距離(の二乗)
	float centerPosLength = (kScreenCenterPos - ConvWorldPosToScreenPos(centerActor.lock()->GetPos())).SqrMagnitude();

	for (auto& actor : inScreenActor)
	{
		auto screenPos = ConvWorldPosToScreenPos(actor->GetPos());

		// こちらの方が近ければ
		float actorToCenterLength = (kScreenCenterPos - ConvWorldPosToScreenPos(actor->GetPos())).SqrMagnitude();
		if (actorToCenterLength > centerPosLength) continue;

		// 更新
		centerActor = actor;
		centerPosLength = actorToCenterLength;
	}

	// 定まったActorにロックオン
	p->m_lockOnActor = centerActor;
}

void PlayerNormal::MoveCameraTarget()
{
	m_player.lock()->m_targetPos = m_player.lock()->GetPos() + kCameraTargetOffset;
}