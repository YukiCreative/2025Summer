#pragma once
#include "VirtualCamera.h"
#include "Vector2.h"

// 回転する
class RotateCamera : public VirtualCamera
{
public:

	RotateCamera();

	void Init(const Vector3& target, const Vector3& targetToCamera, const float cameraDistance);

	void Update() override;

	void AddVel(const Vector2& vel);
	void SetVel(const Vector2& vel);
	void SetCameraDistance(const float dist);

	// すまないがこの操作は削除させていただく
	void SetPos(const Vector3& pos) = delete;

private:

	Vector2 m_vel;

	// 内部的にtargetPosからこのベクトル分だけ伸ばした位置をposに代入する
	// カメラ回転はこれを回転させれば楽に実装できる
	Vector3 m_targetToCamera;
	// 向きと距離を分離すれば距離の変更が楽になる
	float m_lerpedCameraDistance;
	float m_cameraDistance;

private:

	void Rotate();
	void EasingDistance();
};

