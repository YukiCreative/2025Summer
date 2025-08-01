#pragma once
#include "../Utility/RangeLimitedValue.h"
#include "../Geometry/Vector3.h"
#include "../Geometry/Vector2.h"
#include <memory>
#include <list>

class Player;

namespace DxLib
{
	struct tagMATRIX;
}


class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Update();
	void Draw_Debug() const;

	void SetTargetPos(const Vector3& targetPos);
	void SetLerpPos(const Vector3& pos);
	// Y軸基準でtargetPosを中心に回転
	void RotateCameraUpVecY(const float rad);
	// カメラを上下に回転させる
	void RotateCameraV(const float rad);
	// pos→targetPosの単位ベクトル
	Vector3 GetCameraDir() const;
	// XZ平面に限ったカメラの向きを取得
	Vector3 GetCameraDirXZ() const;
	// 与えられたベクトルをカメラのXZ平面の向きに回転させる
	// 第二引数に入れられた座標軸をカメラの奥行として設定する
	// 例：vec2={0,0,1} → z+方向がカメラの奥行になる
	Vector3 RotateVecToCameraDirXZ(const Vector3& vec, const Vector3& vec2);
	void SetTargetFoV(const float deg);
	void SetTargetDistance(const float dist);
	float GetTargetDistance() const;
	float GetTargetDistanceXZ() const;
	Vector3 GetTargetPos() const { return m_targetPos; }
	float GetCameraNearFarLength() const;
	void SetCameraDistanceDefault();
	// カメラの振動
	void SetShake(const int frame, const int strength);

private:
	// どこに向くか
	Vector3 m_targetPos;
	// 実際に見る位置
	Vector3 m_lerpedTargetPos;
	// 注視点から見たカメラの向き
	Vector3 m_targetToCamera;
	// 注視点とカメラの距離
	float m_targetDistance;
	float m_lerpedTargetDistance;
	// 視野角を滑らかに変化させたい
	float m_FoV;
	float m_targetFoV;
	Vector2 m_cameraVel;

	// カメラの振動分
	Vector3 m_shakePos;
	int m_shakeFrame;
	int m_startFrame;
	int m_shakeStrength;

	// カメラが目指す座標
	Vector3 m_targetCameraPos;
	Vector3 m_lerpedCameraPos;

private:
	void Rotate();
	void SetShakePos();
};