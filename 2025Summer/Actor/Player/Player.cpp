#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"
#include "Collidable.h"
#include "Rigid.h"
#include "SphereCollider.h"
#include "Camera.h"
#include <DxLib.h>

namespace
{
	const PhysicalMaterial kPhysiMat =
	{
		0.1f, 0.05f, 0.1f
	};

	const Vector3 kCameraTargetOffset = { 0, 200, 0 };

	constexpr float kRunSpeed    = 0.001f;
	constexpr float kDashSpeed   = 0.002f;
	constexpr float kCameraHSpeed = 0.0001f;
	constexpr float kCameraVSpeed = 0.0001f;
}

Player::Player()
{
}

void Player::Init(const std::weak_ptr<Camera> camera)
{
	m_camera = camera;

	m_model = std::make_shared<AnimationModel>();
	m_model->Init("Data/Model/Player.mv1", 3);

	auto col = std::make_shared<SphereCollider>();
	col->Init(m_pos, 1, false, false, 1);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysiMat);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);
}

void Player::Update()
{
	Move();

	// カメラを動かす
	Vector2 rightAxis = Input::GetInstance().GetRightInputAxis();
	rightAxis.y *= -1;

	m_camera.lock()->RotateCameraUpVecY(rightAxis.x * kCameraHSpeed);
	m_camera.lock()->RotateCameraV(rightAxis.y * kCameraVSpeed);

	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	m_camera.lock()->SetTargetPos(m_pos + kCameraTargetOffset);
	m_model->SetPos(m_pos);
}

void Player::Move()
{
	// 入力で移動
	Vector2 inputAxis = Input::GetInstance().GetLeftInputAxis();
	inputAxis.x *= -1;		// 左右(もしくは上下)が反転しているので直す
	// アナログ入力を生かしたいので正規化はしない

	Vector3 vel = Vector3{ inputAxis.x, 0,inputAxis.y } *kRunSpeed;
	// 移動量をカメラの向きに補正
	vel = m_camera.lock()->RotateVecToCameraDirXZ(vel, Vector3::Back());

	// プレイヤーの現在の向きを取得
	const Vector3 dir = m_model->GetDirection() * -1; // 逆なので反転

	// 速度と向きとの外積をとって、出た値で回転
	Vector3 cross = dir.Cross(vel);
	const float dot = dir.Dot(vel);

	// 真反対に移動しようとすると、挙動がバグるので特別に処理
	if (cross.y <= 0.0001f && dot <= -0.9999f)
	{
		// 向きを無理やり回転させてバグを阻止しとこ
		// 後からでも急旋回状態、作っていいんですよ？
		cross.y += 0.1f;
	}

	m_model->RotateUpVecY(cross.y * 0.3f);

	// 自分の向きと移動方向が乖離していると、移動量が反転する
	// 振り向く際に一瞬反対方向に移動するみたいな
	m_collidable->AddVel(vel * dot);
}

void Player::Draw() const
{
	m_model->Draw();
}

void Player::CommitMove()
{
	// 速度減衰させて取得
	m_pos += m_collidable->UpdateRigid();
}
