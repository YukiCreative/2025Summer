#pragma once
#include "RangeLimitedValue.h"
#include "Vector3.h"
#include <memory>

class Player;

namespace DxLib
{
	struct tagMATRIX;
}

// DxLibにカメラが一つしか存在しないのでシングルトンでも良さげ？
// せずに済むならそれが一番なのだが
class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Update();
	void Draw_Debug() const;

	void SetTargetPos(const Vector3& targetPos);
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

private:
	// どこに向くか
	Vector3 m_targetPos;
	// 実際に見る位置
	Vector3 m_lerpedTargetPos;
	// 注視点から見たカメラの向き
	Vector3 m_targetToCamera;
	// 注視点とカメラの距離
	float m_targetDistance;
	// 視野角を滑らかに変化させたい
	float m_FoV;
	float m_targetFoV;
};