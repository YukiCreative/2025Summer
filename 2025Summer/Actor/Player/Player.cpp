#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"
#include "Collidable.h"
#include "Rigid.h"
#include "CapsuleCollider.h"
#include "Camera.h"
#include <DxLib.h>
#include "PlayerState.h"
#include "PlayerIdle.h"
#include "ActorController.h"
#include "Game.h"

namespace
{
	const PhysicalMaterial kPhysiMat =
	{
		0.1f, 0.05f, 0.1f
	};

	constexpr int kWeight = 100;

	constexpr float kCameraHSpeed = 0.0001f;
	constexpr float kCameraVSpeed = 0.0001f;

	const Vector3 kCapsuleEndPosOffset = {0, 200, 0};
	constexpr float kCapsuleRadius = 30;
	constexpr float kAnimPlaySpeed = 30.0f;
}

Player::Player() :
	m_targetPos(),
	m_isLockOn(false),
	Actor(false)
{
}

void Player::Init(const std::weak_ptr<Camera> camera, std::weak_ptr<ActorController> cont)
{
	m_camera = camera;
	m_cont = cont;

	m_model = std::make_shared<AnimationModel>();
	m_model->Init("Data/Model/Player.mv1", kAnimPlaySpeed);

	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + kCapsuleEndPosOffset, kWeight, false, false, kCapsuleRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysiMat);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_state = std::make_shared<PlayerIdle>(weak_from_this());
}

void Player::Update()
{
	// 状態を更新
	// 切り替わったら違うインスタンスが入ってくる
	m_state = m_state->Update();

	// ロックオンボタンを押したら近くの敵をロックオン
	if (Input::GetInstance().IsTrigger("LockOn"))
	{
		LockOn();
	}

	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	m_model->Update();

	m_camera.lock()->SetTargetPos(m_targetPos);
	m_model->SetPos(m_pos);
}

void Player::CameraMove()
{
	// カメラを動かす
	Vector2 rightAxis = Input::GetInstance().GetRightInputAxis();
	rightAxis.y *= -1;

	m_camera.lock()->RotateCameraUpVecY(rightAxis.x * kCameraHSpeed);
	m_camera.lock()->RotateCameraV(rightAxis.y * kCameraVSpeed);
}

float Player::DefaultGroundDrag()
{
	return kPhysiMat.groundDrag.Value();
}

float Player::DefaultAirDrag()
{
	return kPhysiMat.airDrag.Value();
}

void Player::LockOn()
{
	// もしロックオン中なら、
	if (m_isLockOn)
	{
		// 解除して終わり
		m_isLockOn = false;
		m_lockOnActor.reset();
		return;
	}

	// 画面中央に一番近い、ロックオンできる敵を探す
	auto lockOnList = m_cont.lock()->SearchCanLockOnActor();

	// 画面内にいるか
	ActorList_t inScreenActor;
	for (auto& actor : lockOnList)
	{
		// FIX:バウンディングボックスによる判定に変える
		if (CheckCameraViewClip(actor->GetPos()))
		{
			inScreenActor.emplace_back(actor);
		}
	}

	// とりあえず先頭の要素を入れておく
	std::weak_ptr<Actor> centerActor = inScreenActor.front();
	const Vector3 kScreenCenterPos = {Game::kScreenHalfWidth, Game::kScreenHalfHeight, 0 };
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
	m_lockOnActor = centerActor;
	m_isLockOn = true;
}

void Player::Move(const float moveSpeed)
{
	// 入力で移動
	Vector2 inputAxis = Input::GetInstance().GetLeftInputAxis();
	inputAxis.x *= -1;		// 左右(もしくは上下)が反転しているので直す
	// アナログ入力を生かしたいので正規化はしない

	Vector3 vel = Vector3{ inputAxis.x, 0,inputAxis.y } * moveSpeed;
	// 移動量をカメラの向きに補正
	vel = m_camera.lock()->RotateVecToCameraDirXZ(vel, Vector3::Back());

	// プレイヤーの現在の向きを取得
	const Vector3 dir = m_model->GetDirection() * -1; // 逆なので反転

	// 速度と向きとの外積をとって、出た値で回転
	Vector3 cross = dir.Cross(vel);
	const float dot = dir.Dot(vel);

	// 真反対に移動しようとすると、挙動がバグるので特別に処理
	if (cross.y <= 0.01f && dot <= -0.9999f)
	{
		// 向きを無理やり回転させてバグを阻止しとこ
		// 後からでも急旋回状態、作っていいんですよ？
		cross.y += 0.1f;
	}

	m_model->RotateUpVecY(cross.y * 0.3f);

	const float clampDot = max(dot, -0.1f);

	// 自分の向きと移動方向が乖離していると、移動量が反転する
	// 振り向く際に一瞬反対方向に移動するみたいな
	m_collidable->AddVel(vel * clampDot);
}

void Player::Draw() const
{
	m_model->Draw();

	m_collidable->GetCol().Draw();

	DrawFormatString(300, 0, 0xffffff, "CapsuleCol x:%f,y:%f,z:%f", m_collidable->GetPos().x, m_collidable->GetPos().y, m_collidable->GetPos().z);
}

void Player::OnCollision(std::shared_ptr<Actor> other)
{
	printf("なんか当たってる");
}

void Player::CommitMove()
{
	// 速度減衰させて取得
	const Vector3 vel = m_collidable->UpdateRigid();
	m_pos += vel;
	m_collidable->SetPos(m_pos + kCapsuleEndPosOffset * 0.5f);
}
