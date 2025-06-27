#include "Player.h"
#include "AnimationModel.h"
#include "Input.h"
#include "Collidable.h"
#include "Rigid.h"
#include "CapsuleCollider.h"
#include "Camera.h"
#include <DxLib.h>
#include "PlayerState.h"
#include "PlayerNormal.h"
#include "ActorController.h"
#include "Game.h"
#include "Image.h"
#include "RotateCamera.h"

namespace
{
	const PhysicalMaterial kPhysiMat =
	{
		0.1f, 0.05f, 0.1f
	};

	constexpr int kWeight = 10;

	constexpr float kCameraHSpeed = 0.00001f;
	constexpr float kCameraVSpeed = 0.00001f;

	const Vector3 kCapsuleEndPosOffset = {0, 200, 0};
	constexpr float kCapsuleRadius = 30;
	constexpr float kAnimPlaySpeed = 30.0f;

	const std::string kLockOnCursorFile = "LockOnCursor.png";

	const Vector3 kInitTargetToCamera = Vector3{ 0, 3, 5 }.GetNormalize();
	constexpr float kInitCameraDistance = 400.0f;
}

Player::Player() :
	m_targetPos(),
	Actor(false)
{
}

void Player::Init(std::weak_ptr<ActorController> cont)
{
	m_rotateCamera = std::make_shared<RotateCamera>();
	m_rotateCamera->Init(m_pos + kCameraTargetOffset, kInitTargetToCamera, kInitCameraDistance);

	Camera::GetInstance().AddVCamera(m_rotateCamera);

	m_cont = cont;

	m_model = std::make_shared<AnimationModel>();
	m_model->Init("Data/Model/Player.mv1", kAnimPlaySpeed);

	auto col = std::make_shared<CapsuleCollider>();
	col->Init(m_pos, m_pos + kCapsuleEndPosOffset, kWeight, false, false, kCapsuleRadius);
	auto rigid = std::make_shared<Rigid>();
	rigid->Init(kPhysiMat);

	m_collidable = std::make_shared<Collidable>();
	m_collidable->Init(col, rigid);

	m_lockOnGraph = std::make_shared<Image>();
	m_lockOnGraph->Init(kLockOnCursorFile);

	m_state = std::make_shared<PlayerNormal>(weak_from_this());
}

void Player::Update()
{
	// 状態を更新
	// 切り替わったら違うインスタンスが入ってくる
	m_state = m_state->Update();

	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	m_model->Update();
	m_model->SetPos(m_pos);
}

float Player::DefaultGroundDrag()
{
	return kPhysiMat.groundDrag.Value();
}

float Player::DefaultAirDrag()
{
	return kPhysiMat.airDrag.Value();
}

void Player::Move(const float moveSpeed)
{
	// 入力で移動
	Vector2 inputAxis = Input::GetInstance().GetLeftInputAxis();
	inputAxis.x *= -1;		// 左右(もしくは上下)が反転しているので直す
	// アナログ入力を生かしたいので正規化はしない

	Vector3 vel = Vector3{ inputAxis.x, 0,inputAxis.y } * moveSpeed;
	// 移動量をカメラの向きに補正
	vel = Camera::GetInstance().RotateVecToCameraDirXZ(vel, Vector3::Back());

	// プレイヤーの現在の向きを取得
	const Vector3 dir = m_model->GetDirection() * -1; // 逆なので反転

	// 速度と向きとの外積をとって、出た値で回転
	Vector3 cross = dir.Cross(vel);
	const float dot = dir.Dot(vel);

	// 真反対に移動しようとすると、挙動がバグるので特別に処理
	if (cross.y <= 0.01f && dot <= -0.9999f)
	{
		// 強制的に回す
		cross.y += 0.1f;
	}

	m_model->RotateUpVecY(cross.y * 0.3f);

	const float clampDot = max(dot, -0.1f);

	// 自分の向きと移動方向が乖離していると、移動量が反転する
	// 振り向く際に一瞬反対方向に移動するみたいな
	m_collidable->AddVel(vel * clampDot);
}

void Player::MoveWithoutRotate(const float moveSpeed)
{
	// その名の通り回転しないMove
	// 向きによる移動制限も行わない

		// 入力で移動
	Vector2 inputAxis = Input::GetInstance().GetLeftInputAxis();
	inputAxis.x *= -1;		// 左右(もしくは上下)が反転しているので直す
	// アナログ入力を生かしたいので正規化はしない

	Vector3 vel = Vector3{ inputAxis.x, 0,inputAxis.y } *moveSpeed;
	// 移動量をカメラの向きに補正
	vel = Camera::GetInstance().RotateVecToCameraDirXZ(vel, Vector3::Back());

	m_collidable->AddVel(vel);
}

void Player::Draw() const
{
	//m_model->Draw();

	// ロックオン描画
	if (!m_lockOnActor.expired())
	{
		m_lockOnGraph->Draw({ m_lockOnCursorPos.x, m_lockOnCursorPos.y });
	}

#if _DEBUG
	m_collidable->GetCol().Draw();

	if (!m_lockOnActor.expired())
	{
		DrawLine3D(m_pos + kCapsuleEndPosOffset, m_lockOnActor.lock()->GetPos(), 0xfffffff);
	}
#endif
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

	// ロックオンカーソルの挙動
	if (!m_lockOnActor.expired())
	{
		m_lockOnCursorPos = ConvWorldPosToScreenPos(m_lockOnActor.lock()->GetPos());
	}
}

DxLib::tagMATRIX Player::GetModelMatrix() const
{
	auto mat = m_model->GetMatrix();

	mat.m[0][0] *= -1;
	mat.m[0][1] *= -1;
	mat.m[0][2] *= -1;

	return mat;
}
